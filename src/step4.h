#include "common.h"

using namespace std;

typedef struct TAD {
    vector<vector<int> > bound;
        vector<int> level;
        vector<double> score, mean;
} TAD;

typedef struct DATA {
        string fname;
        vector<vector<double> > x;
        vector<vector<bool> > sel;
        TAD tad;
} DATA;


void dpcall(vector<vector<double> > const &x, vector<vector<double> > const &sx, int st, int ed, int minsz, int maxsz, double penalty, vector<vector<bool> > const &sel, vector<vector<double> > const &tadn, vector<double> const &boundn, double &score, double &mean);
void getBound(int st, int ed, int level, vector<vector<double > > const &x, vector<vector<double > > const &sx, TAD &tad);
void outputTAD(char *fname, TAD const &tad);
void runone(DATA &data, int minsz, int maxsz, double penalty, clock_t timeed, clock_t time0);
void outputBED(char *fname, TAD const &tad, char *chrnum, int res);
