#include "common.h"

using namespace std;

void HiCnorm(vector<vector<double> > &x, int maxsz, bool usemean = false); // Step3
double fdrthreshold(vector<double> const &z, double fdr, int minpkn = 0); // Step3
