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
    bool flag_dec = true;
    bool flag_acc = true;
    vector<int> runs;
    vector<int> runs_addresses;
    vector<int> accumulator;
    int run_counter = 0;
    
    for (int i=1; i <= rr_data.size(); i++) 
    {   
        if (rr_data[i] < rr_data[i-1])
        {
            cout << rr_data[i] << endl; 
            if (flag_dec) {
                flag_dec = false;
                // accumulator[run_counter] += 1;
                flag_acc = true;
                runs_addresses.push_back(i);
                runs.push_back(run_counter);
                run_counter = 0;
            }

            if (flag_acc) {
                flag_acc = false;
                // accumulator[run_counter] += 1;
                flag_acc = true;
                runs_addresses.push_back(i);
                runs.push_back(run_counter);
                run_counter = 0;
            }
        }
    }
    for (int j = 0; j < runs.size(); j++) 
    {
        cout << runs[j] << endl;
    }
    return runs;
}