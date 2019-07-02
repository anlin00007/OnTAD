#include "step4.h"
#include "step2.h"
#include "common.h"
#include <map>
vector<vector<double> > S, M;
vector<vector<int> > B;
/*---------------------------------------------------------------*/
/* Step4: apply DP to assemble boundaries into TADs
 *    Input: distance effect removed data and local minimum
 *    
 *                      **/

void runone(DATA &data, int minsz, int maxsz, double penalty, clock_t timeed, clock_t time0)
{
	int i, j, k, l, L = (int)data.x.size();
	double score, mean;
	vector<vector<bool> > tsel = data.sel;
	vector<vector<double> > tadn; 
	vector<double> boundn;
	vector<vector<double> > x = data.x, sx;
	data.tad = TAD();
	clock_t timest = timeed;
	printf("Cumsum data:\n");fflush(stdout);
	cumsum(x, sx);
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);
	timest = timeed;

	printf("Call TADs:\n");fflush(stdout);
	dpcall(x, sx, 0, 0, minsz, maxsz, penalty, tsel, tadn, boundn, score, mean);	
	TAD mytad;
	getBound(0, L, 0, x, sx, mytad); 
	timeed = clock();
	double tscore = 0;
	if((int)mytad.score.size() > 0) tscore = mytad.score[0];
	printf(" Done %5.3fsec (%5.3fsec) | score=%5.3f, tadn=%d\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6, tscore, (int)mytad.bound.size()); fflush(stdout);
	timest = timeed;

	vector<vector<double> > tx(L, vector<double>(L, 0));
	i = (int)data.tad.bound.size();
	for(j = 0; j < (int)mytad.bound.size(); j++)
	{	for(k = mytad.bound[j][0]; k <= mytad.bound[j][1]; k++)
		for(l = mytad.bound[j][0]; l <= mytad.bound[j][1]; l++)
			tx[k][l] = mytad.mean[j];
		for(k = 0; k < i; k++)
			if(data.tad.bound[k][0] == mytad.bound[j][0] && data.tad.bound[k][1] == mytad.bound[j][1]) break;
		if(k >= i) 
		{	data.tad.bound.push_back(mytad.bound[j]);
			data.tad.level.push_back(mytad.level[j]);
			data.tad.mean.push_back(mytad.mean[j]);
			data.tad.score.push_back(mytad.score[j]);
		}
	}
	for(j = 0; j < L; j++)
		for(k = 0; k < L; k++)
			x[j][k] = max(0., x[j][k]-tx[j][k]);
}

void dpcall(vector<vector<double> > const &x, vector<vector<double> > const &sx, int st, int ed, int minsz, int maxsz, double penalty, vector<vector<bool> > const &sel, vector<vector<double> > const &tadn, vector<double> const &boundn, double &score, double &mean)
{	
	int i, j, k;

	if(ed == 0)
	{	S.clear();
		S.resize((int)sx.size(), vector<double>((int)sx[0].size(), -1e6));
		M.clear();
		M.resize((int)sx.size(), vector<double>((int)sx[0].size(), 0));
		B.clear();
		ed = (int)sx.size();
	}

	int l = ed - st;
	if(S[st][ed - 1] > -1e6)
	{	score = S[st][ed - 1];
		mean = M[st][ed - 1];
		return;
	}
	
	double ssum = sx[ed - 1][ed - 1];
	if(st > 0) ssum = ssum - sx[st - 1][ed - 1] - sx[ed - 1][st - 1] + sx[st - 1][st - 1];
	double sn = l * l;

	vector<int> trace;
	if(l <= max(1, minsz))
	{	score = 0;
		mean = ssum / sn;
	}
	else
	{	vector<double> rtscore(l, -1e6);
		rtscore[0] = 0;
		vector<int> ttrace(l, -1);
		for(i = 1; i < l; i++)
		{	if(i < l - 1)
			{	for(j = i + 1; j < l; j++)
					if(sel[st + i][st + j]) break;
				if(j >= l) continue;
			}
			vector<double> tscore(i, -1e6);
			bool flag = false;
			k = 0;
			for(j = i - 1; j >= 0; j--)
			{	if(!sel[st+j][st+i]) continue;
				if(flag && i - j > maxsz) break; 
				flag = true;
				
				double ts, tm;
				if(j == 0 && i == l - 1)
				{	ts = 0; 
					tm = ssum / sn;
				}
				else
				{	dpcall(x, sx, st + j, st + i + 1, minsz, maxsz, penalty, sel, tadn, boundn, ts, tm);
				}
				tscore[j] = ts;
				if(j > 0) tscore[j] += rtscore[j];
				if(k < 0 || tscore[k] <= tscore[j]) k = j;
			}
			rtscore[i] = tscore[k];
			ttrace[i] = k;
			if(ed == (int)sx.size()) printProgress((double)(i + 1) / (double)l);
		}
	
		double tsum = 0, tn = 0;
		j = l - 1;
		bool flag = false;
		while(j > 0)
		{	k = ttrace[j];
			if(k > 0) trace.push_back(k);
			if(rtscore[j] - rtscore[k] > 0)
			{	tsum += sx[st + j][st + j];
				if(st + k > 0) tsum = tsum - sx[st + k - 1][st + j] - sx[st + j][st + k - 1] + sx[st + k - 1][st + k - 1];
				tn += (j - k + 1) * (j - k + 1);
				if(flag)
				{	tsum -= x[st + j][st + j];
					tn--;
				}
				flag = true;
			}
			else flag = false;
			j = k;
		}
		mean = (ssum - tsum) / (sn - tn + 1e-5);
		score = rtscore[l - 1];
	}

	double bsuml = 0, bsumr = 0, bnl = 1e-5, bnr = 1e-5;
	if(st > 0)
	{	int ta = max(0, st - l + 1);
		bsuml += sx[st - 1][ed - 1];
		if(ta > 0) bsuml = bsuml - sx[ta - 1][ed - 1] - sx[st - 1][st] + sx[ta - 1][st];
		bnl += (st - ta) * (l - 1);
	}
	if(ed < (int)sx.size())
	{	int ta = min((int)sx.size() - 1, ed + l - 2);
		bsumr += sx[ed - 2][ta];
		if(st > 0) bsumr = bsumr - sx[st - 1][ta] - sx[ed - 2][ed - 1] + sx[st - 1][ed - 1];
		bnr += (l - 1) * (ta - ed + 1);
	}
	double delta = mean - max(bsuml / bnl, bsumr / bnr);
	score = score + delta;
	if((int)tadn.size() > 0) score += penalty / 4. * (boundn[st] + boundn[ed - 1] + tadn[st][ed - 1] * 2.);
	score -= penalty;
//	score = max(score, -penalty * (0.5 + (double)(l - 1) / 1e4));
	score = max(0., score);
	S[st][ed - 1] = score;
	M[st][ed - 1] = mean;
	for(i = 0; i < (int)trace.size(); i++)
	{	vector<int> tt(2, st + trace[i]);
		tt[0] = st * (int)sx.size() + ed;
		B.push_back(tt);
	}
}

void getBound(int st, int ed, int level, vector<vector<double > > const &x, vector<vector<double > > const &sx, TAD &tad)
{	
	double s, n;
	s = sx[ed - 1][ed - 1];
	if(st > 0) s = s - sx[st - 1][ed - 1] - sx[ed - 1][st - 1] + sx[st - 1][st - 1];
	n = (ed - st) * (ed - st);
	int l = st * (int)sx.size() + ed;

	int i, j;
	vector<int> loc;
	for(i = 0; i < (int)B.size(); i++)
	{	if(B[i][0] == l)
			loc.push_back(B[i][1]);
	}
	sort(loc.begin(), loc.end());
	if((int)loc.size() > 0)
	{	loc.insert(loc.begin(), st);
		loc.push_back(ed - 1);
		bool flag = false;
		for(i = (int)loc.size() - 2; i >= 0; i--)
		{	getBound(loc[i], loc[i + 1] + 1, level + 1, x, sx, tad);	
			if(S[loc[i]][loc[i+1]] > 0)
			{	double ts = sx[loc[i+1]][loc[i+1]];
				if(loc[i] > 0) ts = ts - sx[loc[i] - 1][loc[i + 1]] - sx[loc[i + 1]][loc[i] - 1] + sx[loc[i] - 1][loc[i] - 1];
				s -= ts;
				double tn = (loc[i + 1] - loc[i] + 1) * (loc[i + 1] - loc[i] + 1);
				n -= tn;
				if(flag)
				{	s += x[loc[i + 1]][loc[i + 1]];
					n++;
				}
				flag = true;
			}
			else flag = false;
		}
	}

	if(S[st][ed - 1] > 0)
	{	vector<int> tb(2);
		tb[0] = st; tb[1] = ed - 1;
		tad.bound.push_back(tb);
		tad.level.push_back(level);
		tad.score.push_back(S[st][ed - 1]);
		tad.mean.push_back(M[st][ed - 1]);
	}	

	if(st == 0 && ed == (int)sx.size())
	{	reverse(tad.bound.begin(), tad.bound.end());
		reverse(tad.level.begin(), tad.level.end());
		reverse(tad.score.begin(), tad.score.end());
		reverse(tad.mean.begin(), tad.mean.end());
	}
}

void outputTAD(char *fname, TAD const &tad)
{
	int j;
	FILE *f = fopen(fname, "w");
	for(j = 0; j < (int)( tad.bound.size() ); j++)
	{	fprintf(f, "%d\t%d\t%d\t%5.3f\t%5.3f\n", tad.bound[j][0] + 1, tad.bound[j][1] + 1, tad.level[j], tad.mean[j], tad.score[j]); 
	}
	fclose(f);
}

void outputBED(char *fnamebed, TAD const &tad, int chrnum, int chrlength,int res)
{
        int j;
	int tl;
	int endp;
	std::map <int, const char*> color_list;
	color_list[1] = "56,108,176";
	color_list[2] = "127,201,127";
	color_list[3] = "190,174,212";
	color_list[4] = "253,192,134";
	color_list[5] = "255,0,0";
        FILE *bedf = fopen(fnamebed, "w");
	fprintf(bedf,"track name=\"OnTAD chr%d\" description=\"OnTAD chr%d\" visibility=2 itemRgb=\"On\"\n", chrnum, chrnum);
        for(j = 1; j < (int)( tad.bound.size() ); j++)
        {       
		if (tad.level[j] >=5) 
		   tl = 5;
		else
		   tl = tad.level[j];
		if ((tad.bound[j][1] + 1)*res > chrlength)
		   endp = chrlength;
		else
		   endp = (tad.bound[j][1] + 1)*res;
		fprintf(bedf, "chr%d\t%d\t%d\t%d\t0\t.\t%d\t%d\t%s\n", chrnum, (tad.bound[j][0] + 1)*res, endp, j, (tad.bound[j][0] + 1)*res, endp, color_list[tl]);
        }
        fclose(bedf);
}
