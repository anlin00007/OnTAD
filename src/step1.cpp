#include "step1.h"
/*---------------------------------------------------------------*/
/* Step1: read input matrix
 *    Input: matrix file
 *    Requirement: input file must be a N*N matrix, seperated by TAB.
 *                      **/

int num_space(char* str, int len){
	int num = 0;
	int i;

	for(i=0; i<len; i++){
		if(str[i] == ' ' || str[i] == '\t') num++;
	}

	printf("in %s, return num of column = %d\n", __FUNCTION__, num+1);
	return (num+1);
}

void loadMatrix(char const *fname, vector<vector<double> > &x, int maxsz) //read input file line by line and only store the value near diagonal
{
	int i, j, k, l, n = 0, L = 0;
	char *tmp = new char[10000000];
	x.clear();

	FILE *f = fopen(fname, "r");
	if(f == NULL) printf("Cannot open %s\n", fname), exit(0); //stop if cannot find file
	int nrow = 0; //name a variable to store row number
	while(fgets(tmp, 1e7, f) != NULL) //read the file line by line with maximum string in each line as 1e7
	{	l = (int)strlen(tmp); //the number of bytes of each row

		vector<double> tx(L, 0);
		k = 0;
		for(j = 0; j < l; j++) 
		{	if(L == 0) 
			{	if(k > maxsz) tx.push_back(0);
				else tx.push_back(atof(&tmp[j]));
				k++;
			}
			else 
			{	if(k >= n - maxsz && k <= n + maxsz) tx[k] = atof(&tmp[j]);
				k++;
				if(k > n + maxsz) break;
			}
			for(j = j + 1; j < l; j++) if(tmp[j] == ' ' || tmp[j] == '\t') break;
		}
		if(L == 0) L = k;
		printProgress((double)(n + 1) / (double)L);
		if((int)x.size() == 0) x.resize(L);
		x[n++] = tx;
		nrow++;
	}
	if(nrow!=num_space(tmp, l)) {
                        printf("Input doesn't match N*N format or was not seperated by TAB or space!");
                        exit(-3);//stop if input is not a N*N matrix
        }
	fclose(f);
	delete []tmp;
}
