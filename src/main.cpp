#include "./step1.h"
#include "./step2.h"
#include "./step3.h"
#include "./step4.h"
#include "./common.h"

using namespace std;
clock_t time0, timest, timeed;
/*---------------------------------------------------------------*/
/* Func name: processData
   Input: data
   Output: data with distance effect removed
**/
void processData(DATA &data, int maxsz, bool usemean)
{
	int i, j, k, l;
	vector<vector<double> > sx, sc;
	vector<vector<bool> > lm;	

	timest = timeed;
	printf("Cumsum data:\n");fflush(stdout);
	cumsum(data.x, sx);
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);
	timest = timeed;
	
	printf("Calculate score:\n"); fflush(stdout);
	getScore(sx, maxsz, sc); fflush(stdout);
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);
	timest = timeed;

	printf("Find local min:\n"); fflush(stdout);
	calMins(sc, lm);
	setPair(lm, data.sel);
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);
	timest = timeed;
		
	printf("Normalize data: "); fflush(stdout);
	HiCnorm(data.x, maxsz * 2, usemean);
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);
	timest = timeed;
}

/*---------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	int i, j, k, l; 
	int maxsz = 200, minsz = 3;
	double penalty = 0.1;
	
	string fin;
	char const *foutpref = "";
	bool usemean = true;

	bool input_in = false;
        for(i = 1; i < argc; i++)
        {
            if(strcmp(argv[i], "-penalty") == 0)
            {       penalty = atof(argv[i + 1]);
                i++;
            }
            else if(strcmp(argv[i], "-maxsz") == 0)
            {       maxsz = max(10, atoi(argv[i + 1]));
                i++;
            }
            else if(strcmp(argv[i], "-minsz") == 0)
            {       minsz = max(1, atoi(argv[i + 1]));
                i++;
            }
            else if(strcmp(argv[i], "-o") == 0)
            {       foutpref = argv[i + 1];
                i++;
            }
            else
            {       
        	if(!input_in){
        		fin = string(argv[i]);
        		input_in = true;
        	} else{ // already have input; but user type in the 2nd input
        		printf("Error: One and only one input is allowed\n");
				return -1;
        	}
            }
        }

	time0 = clock();
	printf("\nOnTAD v1.0:\nmaxsz=%d, minsz=%d, penalty=%5.3f\n\n", maxsz, minsz, penalty);
	DATA data;

	data.fname = fin;
	timest = clock();
	printf("Load %s:\n", fin.c_str()); fflush(stdout);
	loadMatrix(fin.c_str(), data.x, maxsz * 2);
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);
	processData(data, maxsz, usemean);
	printf("\n");
	timeed = clock();
	runone(data, minsz, maxsz, penalty, timeed, time0);

	char fout[1000];
	sprintf(fout, "%s.tad", data.fname.c_str());
	outputTAD(fout, data.tad);

	printf("Completed!\n\n");
	timeed = clock();
	printf("Total run time: %5.3fsec\n\n", (double)(timeed-time0)/1e6); fflush(stdout);

	return(0);
}
