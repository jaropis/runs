#include "runs.h"
#include <iostream>
#include <fstream>

using namespace std;

RRRuns::RRRuns(string path)
{
    ifstream rrfile;
    rrfile.open(path);
    double current_rr;
    int current_flag;
    while(!rrfile.eof()) 
    {
        rrfile >> current_rr;
        rrfile >> current_flag;
        this->rr_data.push_back(current_rr);
        this->annotations.push_back(current_flag);
    }
    for (int idx = 0; idx < rr_data.size(); idx++) 
    {
        cout << idx << " " 
             << rr_data[idx] 
             << " " <<annotations[idx] 
             << endl;
    }
}

vector<int> RRRuns::get_runs()
{   
    vector<int> runs;
    int runs_addresses [rr_data.size()][3]; //this holds addresses of decelerations - the first row contains the address of the beginning, the second the length of the run, and the third the type of run (-1 is acc, 0 is neutral, 1 is dec)
    int accumulator_dec [100]; // accumulates statistics for acceleration runs
    int accumulator_acc [100]; // accumulates statistics for deceleration runs
    bool flag_dec = true;
    bool flag_acc = true;
    int index_dec;
    int index_acc;
    int run_counter = 0;
    int running_rr_number = 0;
    /*
    for (int i=1; i <= rr_data.size(); i++) 
    {   
       if(flag_dec && rr_data[i-1] < rr_data[i]) 
       {
           index_dec++;
       }
       if(flag_dec && rr_data[i-1] > rr_data[i])
       {
           index_acc++;
       }
        if(accumulator_dec && rr_data[i-1] >= rr_data[i])
        {
            accumulator_dec[index_dec] += 1;
            // runs_addresses[index_dec];
        }
    }
    for (int j = 0; j < runs.size(); j++) 
    {
        cout << runs[j] << endl;
    }
    return runs; */
    return runs;
}