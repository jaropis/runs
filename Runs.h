#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

class RRRuns {
    public:
        RRRuns(std::string path, bool write_last_run);
        vector<int> runs;
        vector<int> get_runs();   
        std::map<std::string, std::vector<int>> full_runs;
    private:
        vector<double> rr_data;
        vector<int> annotations;
        vector<vector<int>> runs_addresses;  //this holds addresses of decelerations - the first row contains the address of the end, the second the length of the run, and the third the type of run (-1 is acc, 0 is neutral, 1 is dec)
        bool write_last_run;
        void update_runs_addresses(vector<int> new_entry);
        void print_addresses(int how_many, int max_length);        
        // vector<int> rollup_runs(int accumulator_dec[], int accumulator_acc[], int accumulator_neu[]);
};