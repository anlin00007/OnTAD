#include "common.h"

using namespace std;

void loadMatrix(char const *fname, vector<vector<double> > &x, int maxsz); // Step1
void loadMatrixFromHiC(
	string fname,
	vector<vector<double> > &x,
	int maxsz,
	string hic_norm,
	int resolution,
	char const* chrnum,
	int chrlength);
