#include "step3.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sys/stat.h>
#define GSL_DLL
#include <gsl/gsl_cdf.h> //use gaussian p-value function
/*---------------------------------------------------------------*/
/* Step3: remove distance effect
 *    Input: data matrix
 *    requirement: gsl
 *                      **/
void HiCnorm(vector<vector<double> > &x, int maxsz, bool usemean)
{	int i, j, k, l = (int)x.size();
	double fdr = 0.05;
	int refid = 0;
	if(!usemean)
	{
		vector<double> a;
		for(i = 0; i < min(l, maxsz) / 2; i++)
		{	vector<double> tx(l - i);
			for(j = 0; j < l - i; j++) tx[j] = x[j][j + i];	
			double f = fdrthreshold(tx, fdr);
			k = 0;
			for(j = 0; j < (int)tx.size(); j++) if(tx[j] >= f) k++;
			a.push_back((double)k / (double)tx.size());
		}
		vector<double> na((int)a.size(), 0);
		for(i = 0; i < (int)a.size(); i++)
		{	k = 0;
			for(j = max(0, i - 5); j < min((int)a.size(), i + 5); j++)
			{	na[i] += a[j];
				k++;		
			}
			na[i] /= (double)k;
			if(na[refid] < na[i]) refid = i;
			if(i >= 100) break;
		}
		printf("refid = %d", refid);
	}
	printf("\n");
		
	vector<double> rx;
	if(!usemean)
	{	rx.resize(l - refid);
		for(j = 0; j < l - refid; j++) rx[j] = x[j][j + refid];
	}
	double ss = 0;
	for(i = 0; i < min(l - 1, maxsz); i++)
	{	printProgress((double)(i + 1) / (double)min(l - 1, maxsz));
		vector<double> tx(l - i);
		for(j = 0; j < l - i; j++) tx[j] = x[j][j + i];
		double tm = 0, ts = 0;
		for(j = 0; j < l - i; j++) 
		{	x[j][j + i] = x[j + i][j] = tx[j];
			tm += tx[j];
			ts += tx[j] * tx[j];	
		}
		tm /= (double)(l - i);
		ts = sqrt(max(0.01, ts / (double)(l - i) - tm * tm));
		if(usemean)
		{	for(j = 0; j < l - i; j++) 
			{	x[j][j+i]=(x[j][j+i]-tm)/ts + 1.;
				if(i > 0) x[j+i][j]=(x[j+i][j]-tm)/ts + 1.;
			}
		}
		ss += ts;
	}
	if(!usemean)
	{	ss /= (double)i;
		for(i = 0; i < l; i++)
			for(j = 0; j < l; j++)
				x[i][j] /= ss;
	}
}

double fdrthreshold(vector<double> const &z, double fdr, int minpkn) //
{	double threshold;
	int k, l = (int)z.size(), el = l, ol = l + 1;	
	double m = 0, s = 0;
	for(k = 0; k < l; k++)
	{	m += z[k];
		s += z[k] * z[k];
	}
	m = m / (double)l;
	s = sqrt(max(0.01, s / (double)l - m * m));

	while(el < ol)
	{	ol = el;
		threshold = gsl_cdf_gaussian_Qinv(min(0.95, fdr * (double)(l - el + 1) / (double)el), 1.);
		threshold = threshold * s + m;
		el = l;
		for(k = 0; k < l; k++)
			if(z[k] >= threshold) el--;
	}
	
	if(l - el < min(l, minpkn))
	{	vector<double> tz = z;
		sort(tz.begin(), tz.end());
		threshold = tz[max(0,l - minpkn)]; 
	}

	return(threshold);
}
