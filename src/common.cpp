#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sys/stat.h>
//#define GSL_DLL
//#include <gsl/gsl_cdf.h> //use gaussian p-value function
#define PBSTR "........................."
#define PBWIDTH 25 
using namespace std;

void printProgress(double percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}

