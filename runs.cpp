#include "runs.h"
#include <iostream>

using namespace std;

RRRuns::RRRuns(string path)
{
    this->rr_data = { 858.125, 875.625, 833.125, 821.25, 803.75, 798.75, 773.125, 721.875, 730, 816.875, 871.25, 858.125, 839.375, 877.5, 901.875, 885.625, 869.375, 802.5, 760.625, 794.375, 830, 828.75, 774.375, 720, 708.75, 780.625, 743.75, 721.25, 782.5, 849.375, 865};
    this->annotations = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}

vector<int> RRRuns::get_runs()
{   
    int runs_addresses_dec [rr_data.size()][3]; //this holds addresses of decelerations - the first row contains the address of the beginning, the second the length of the run, and the third the type of run (-1 is acc, 0 is neutral, 1 is dec)
    int accumulator_dec [100]; // accumulates statistics for acceleration runs
    int accumulator_acc [100]; // accumulates statistics for deceleration runs
    bool flag_dec = true;
    bool flag_acc = true;
    int index_dec;
    int index_acc;
    int run_counter = 0;
    int running_rr_number = 0;
    
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
            accumulator_dec[index_dec] += 1

        }
    }
    for (int j = 0; j < runs.size(); j++) 
    {
        cout << runs[j] << endl;
    }
    return runs;
}