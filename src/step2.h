#include "common.h"

using namespace std;

void cumsum(vector<vector<double> > const &x, vector<vector<double> > &sx); // Step2
void getScore(vector<vector<double> > const &sx, int maxsz, vector<vector<double> > &score); // Step2
void calMins(vector<vector<double> > const &score, vector<vector<bool> > &lm, int hsz, double ldiff); // Step2
void setPair(vector<vector<bool> > const &lm, vector<vector<bool> > &sel); // Step2
