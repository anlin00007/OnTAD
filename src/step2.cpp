#include "step2.h"
/*---------------------------------------------------------------*/
/* Step2: calculate local minimum
 *    Input: data from step1
 *    
 *                      **/

void cumsum(vector<vector<double> > const &x, vector<vector<double> > &sx) //cumsum the value vertically and horizontally, then the value of each cell (i,j) represent the sum of matrix[0:i,0:j]
{	int i, j, l = (int)x.size();
	sx = x;
	for(i = 0; i < l; i++)
	{	printProgress((double)(i + 1) / (double)l);
		for(j = 1; j < l; j++) sx[i][j] += sx[i][j - 1];
		if(i > 0) 
		{	for(j = 0; j < l; j++)
				sx[i][j] += sx[i - 1][j];
		}
	}	
}

void getScore(vector<vector<double> > const &sx, int maxsz, vector<vector<double> > &score) //use matrix[0:i,0:(i+j)] minus matrix[0:i,0:i] and minus (matrix[0:i-j,0:i+j]-matrix[0:i-j,0:i])
{	int i, j, k, l = (int)sx.size();
	
	score.clear();
	score.resize(l, vector<double>(maxsz, 0));
	
	for(i = 1; i < l; i++)
	{	printProgress((double)(i + 1) / (double)l);
		for(j = 0; j < maxsz; j++)
		{	score[i][j] = sx[i - 1][min(l - 1, i + j + 1)] - sx[i - 1][i];
			if(i - j - 2 >= 0) score[i][j] += - sx[i - j - 2][min(l - 1, i + j + 1)] + sx[i - j - 2][i]; 
			k = min(l - i, j + 1) * min(i, j + 1);
			score[i][j] /= (double)k;
		}
	}	
}

void calMins(vector<vector<double> > const &score, vector<vector<bool> > &lm, int hsz, double ldiff) //compare score for each bin with adjacent bins and determine local minimum
{	int l = (int)score.size(), n = (int)score[0].size();
	//int hsz =5;
	int i, j, k;
	lm.clear();
	lm.resize(l, vector<bool>(n, false));

	vector<int> map;
	for(i = 0; i < l; i++)
	{	double m = 0, s = 0;
		for(j = 0; j < n; j++)
		{	m += score[i][j];
			s += score[i][j] * score[i][j];
		}
		if(s > m * m / (double)n) map.push_back(i);
	}
	if((int)map.size() < 2) return;

	for(i = 0; i < n; i++)
	{	printProgress((double)(i+1) / (double)n);
		double m = 0, s = 0;
		for(j = 1; j < (int)map.size(); j++)
		{	m += score[map[j]][i] - score[map[j - 1]][i];
			s += pow(score[map[j]][i] - score[map[j - 1]][i], 2.);
		}
		m /= (double)((int)map.size() - 1);
		s = sqrt(s / (double)((int)map.size() - 1) - m * m);
		double cut = s * ldiff;/// sqrt((double)hsz) * 3.89; ///1,96std from mean
		for(j = 0; j < (int)map.size(); j++)
		{	double mins = score[map[j]][i], maxs = mins;
			for(k = max(0, j - hsz); k < min((int)map.size(), j + hsz + 1); k++)
			{	if(mins >= score[map[k]][i] && k != j) break;
				maxs = max(maxs, score[map[k]][i]);
			}
			if(k >= min(l, j + hsz + 1) && maxs - mins > cut)
				lm[map[j]][i] = 1;	
		}
		lm[0][i] = lm[l - 1][i] = 1;
	}
}

void setPair(vector<vector<bool> > const &lm, vector<vector<bool> > &sel) //create a sel matrix that mark local minimum bins on diag and edges.
{	int i, j, k, l = (int)lm.size(), n = (int)lm[0].size();	
	sel.clear();
	sel.resize(l, vector<bool>(l, false));
	
	for(i = 0; i < n; i++)
	{	vector<int> map;
		for(j = 0; j < l; j++)
		{	if(lm[j][i]) map.push_back(j);
		}
		for(j = 0; j < (int)map.size(); j++)
		{	for(k = 1; k <= 5; k++)
			{	if(j + k >= (int)map.size()) break;
				sel[0][map[j]] = sel[0][map[j+k]] = sel[map[j]][l-1] = sel[map[j+k]][l-1] = sel[map[j]][map[j+k]] = 1;
			}
		}
	}
}
