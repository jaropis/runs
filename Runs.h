#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class RRRuns {
    public:
        RRRuns(std::string path, bool write_last_run);
        vector<int> get_runs();
    private:
        vector<double> rr_data;
        vector<int> annotations;
        bool write_last_run;
};