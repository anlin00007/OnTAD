#include "./step1.h"
#include "./step2.h"
#include "./step3.h"
#include "./step4.h"
#include "./common.h"
#include <regex>
#include <algorithm>

using namespace std;
clock_t time0, timest, timeed;
/*---------------------------------------------------------------*/
/* Func name: processData
   Input: data
   Output: data with distance effect removed
**/
void processData(DATA &data, int maxsz, int hsz, double ldiff)
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
	calMins(sc, lm, hsz,ldiff);
	setPair(lm, data.sel);
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);
	timest = timeed;
		
	printf("Normalize data: "); fflush(stdout);
	HiCnorm(data.x, maxsz * 2);fflush(stdout);
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);
	timest = timeed;
}

/*---------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	int i, j, k, l; 
	int maxsz = 200, minsz = 3, hsz=5;
	double penalty = 0.1, ldiff=1.96;
	
	string fin;
	char const *foutpref = "";
	bool takelog2 = false;
	char *chrnum;
	//int chrnum;
	int res;
	int chrlength;
	bool bedout = false;
	bool input_in = false;
	bool shuffle = false;

	bool input_hic = false;
	string hic_ending = ".hic";
	string hic_norm = "NONE";
	string allowed_norms[4] = {"NONE", "VC", "VC_SQRT", "KR"};
	bool norm_is_allowed = false;

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
            else if(strcmp(argv[i], "-lsize") == 0)
            {       hsz = atoi(argv[i + 1]);
                i++;
            }
            else if(strcmp(argv[i], "-ldiff") == 0)
            {       ldiff = atof(argv[i + 1]);
                i++;
            }
            else if(strcmp(argv[i], "-log2") == 0)
            {       takelog2 = true;
            }
            else if(strcmp(argv[i], "-shuffle") == 0)
            {       shuffle = true;
            }
	    else if(strcmp(argv[i], "-hic_norm") == 0)
            {   hic_norm = string(argv[i + 1]);
                input_hic = true;
                for (int i = 0; i < (sizeof(allowed_norms)/sizeof(string)); i++) {
                	if(hic_norm.compare(allowed_norms[i]))
                	{
                		norm_is_allowed = true;
                	}
        
                }
                if(!norm_is_allowed) {
                	printf("Error: hic_norm must be one of NONE/VC/VC_SQRT/KR");
        		return -1;
                }
                       i++;
            }
            else if(strcmp(argv[i], "-o") == 0)
            {       foutpref = argv[i + 1];
                i++;
            }
	    else if(strcmp(argv[i], "-bedout") == 0)
	    {   bedout = true;
                chrnum = argv[i+1];
                regex chrreg("(.*)");  // allow chr name in any form, e.g. "chr1" or just "1"
		if ( regex_match(chrnum, chrreg) ){
			i++;
		}else{
			printf("Error: chrnum is required and must be valid\n");
			return -1;
		}

                chrlength = atoi(argv[i+1]);
                regex chrlengthreg("[[:digit:]]+");
                if ( regex_match(argv[i+1], chrlengthreg) ){
                        i++;
                }else{
                        printf("Error: chrlength is required and must be valid\n");
                        return -1;
                }

		res = atoi(argv[i+1]);
		regex resreg("[[:digit:]]+");
                if ( regex_match(argv[i+1], resreg) ){
                        i++;
                }else{
                        printf("Error: resolution is required and must be valid\n");
                        return -1;
                }
	    }
            else
            {       
        	if(!input_in){
        		fin = string(argv[i]);
                        if(hasEnding(fin, hic_ending)) {
                        	input_hic = true;
                        }
        		input_in = true;
                } else { // already have input; but user type in the 2nd input
        		printf("Error: One and only one input is allowed\n");
				return -1;
        	}
            }
        }
        if (input_hic & (!bedout)) {
                printf("Error: When specify hic input, must specify the outputBED.\n");
                return -1;
        }

	time0 = clock();
	printf("\nOnTAD v1.4:\nmaxsz=%d, minsz=%d, penalty=%5.3f, lsize=%d, ldiff=%f\n\n", maxsz, minsz, penalty, hsz, ldiff);
	DATA data;
	
	
	data.fname = fin;
	timest = clock();
	printf("Load %s:\n", fin.c_str()); fflush(stdout);
        if (input_hic) {
                loadMatrixFromHiC(fin, data.x, maxsz, hic_norm, res, chrnum, chrlength);
        } else {
	        loadMatrix(fin.c_str(), data.x, maxsz * 2);
        }

	if(takelog2)
        {for(j = 0; j < (int)data.x.size(); j++)
            for(k = 0; k < (int)data.x.size(); k++)
               data.x[j][k] = log2(data.x[j][k] + 1.);
        }
	timeed = clock();
	printf(" Done %5.3fsec (%5.3fsec)\n", (double)(timeed-timest)/1e6, (double)(timeed-time0)/1e6); fflush(stdout);

	if(shuffle)
	{
	srand(time(0));
	printf("shuffling matrix"); fflush(stdout);
        int L=(int)data.x.size();
        for(int I=0;I<=maxsz;I++)
	{
          for(int j=0;j<L*10;j++)
  	  { int index1= rand()%(L - I);
    	    int index2= rand()%(L - I);
         
            swap(data.x[index1][index1+I],data.x[index2][index2+I]);
            swap(data.x[index1+I][index1], data.x[index2+I][index2]);
           }
        }
	}

	processData(data, maxsz, hsz, ldiff);
	printf("\n");
	timeed = clock();
	runone(data, minsz, maxsz, penalty, timeed, time0);

	char fout[std::max(strlen(foutpref),strlen(data.fname.c_str()))+10];
	char foutbed[std::max(strlen(foutpref),strlen(data.fname.c_str()))+10];
        if(foutpref == ""){
            sprintf(fout, "%s.tad", data.fname.c_str());
	}
       	else{ 
	    sprintf(fout, "%s.tad", foutpref);
	}
	outputTAD(fout, data.tad);
	
	if(bedout == true){
	    if(foutpref == ""){
		sprintf(foutbed, "%s.bed", data.fname.c_str());
	    }
	    else{ 
		sprintf(foutbed, "%s.bed", foutpref);
	    }
	    outputBED(foutbed, data.tad, chrnum, chrlength, res);	
	}
	printf("Completed!\n\n");fflush(stdout);
	printf("Output to %s\n\n", fout);fflush(stdout);
	timeed = clock();
	printf("Total run time: %5.3fsec\n\n", (double)(timeed-time0)/1e6); fflush(stdout);

	return(0);
}
