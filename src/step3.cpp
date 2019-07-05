#include "step3.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sys/stat.h>

/*---------------------------------------------------------------*/
/* Step3: remove distance effect
 *    Input: data matrix
 *    
 *                      **/
void HiCnorm(vector<vector<double> > &x, int maxsz)
{	int i, j, k, l = (int)x.size();
	double fdr = 0.05;
	int refid = 0;
	printf("\n");

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
		ts = sqrt(max(1e-6, ts / (double)(l - i) - tm * tm));
		for(j = 0; j < l - i; j++) 
			{	x[j][j+i]=(x[j][j+i]-tm)/ts + 1.;
				if(i > 0) x[j+i][j]=(x[j+i][j]-tm)/ts + 1.;
			}
		ss += ts;
	}
}

