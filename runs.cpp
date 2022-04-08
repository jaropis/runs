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
    // cout << "data size: " << rr_data.size();
    for (int idx = 0; idx < rr_data.size(); idx++) 
    {
       /* cout << idx << " " 
             << rr_data[idx] 
             << " " <<annotations[idx] 
             << endl; */
    }
}

vector<int> RRRuns::get_runs()
{   
    vector<int> runs;
    int runs_addresses [rr_data.size()][3]; //this holds addresses of decelerations - the first row contains the address of the beginning, the second the length of the run, and the third the type of run (-1 is acc, 0 is neutral, 1 is dec)
    bool flag_dec = false;
    bool flag_acc = false;
    bool flag_neu = false;
    int index_dec = 0;
    int index_acc = 0;
    int index_neu = 0;
    int run_counter = 0;
    int running_rr_number = 0;
    int current_address = 0; // holds the address in runs_addresses array, adds consecutive runs
    int accumulator_dec [rr_data.size()]; // accumulates statistics for acceleration runs
    int accumulator_acc [rr_data.size()]; // accumulates statistics for deceleration runs
    int accumulator_neu [rr_data.size()]; // accumulates statistics for deceleration runs
    cout << "data size: " << rr_data.size() << endl;
    for (int i=1; i < rr_data.size(); i++) 
    {  
       if(flag_dec && rr_data[i-1] < rr_data[i]) 
       {
           index_dec++;
           flag_dec = true;
       }
       
       if(flag_acc && rr_data[i-1] > rr_data[i])
       {
           index_acc++;
           flag_acc = true;
       }
       if(flag_neu && rr_data[i-1] == rr_data[i])
       {
           index_acc++;
           flag_neu = true;
       }
        if(flag_dec && rr_data[i-1] > rr_data[i])
        {   
            accumulator_dec[index_dec] += 1;
            runs_addresses[current_address][0] = running_rr_number;
            runs_addresses[current_address][1] = index_dec;
            runs_addresses[current_address][2] = 1;
            current_address++;
            running_rr_number++;
            index_dec = 0;     
            flag_dec = false;
            flag_acc = true;               
        }
        if(flag_acc && rr_data[i-1] < rr_data[i])
        {
            accumulator_acc[index_acc] += 1;
            runs_addresses[current_address][0] = running_rr_number;
            runs_addresses[current_address][1] = index_acc;
            runs_addresses[current_address][2] = -1;
            current_address++;
            running_rr_number++;
            flag_acc = false;
            flag_dec = true;
        }
        if(flag_neu && rr_data[i-1] != rr_data[i])
        {
            accumulator_neu[index_neu] += 1;
            runs_addresses[current_address][0] = running_rr_number;
            runs_addresses[current_address][1] = index_neu;         
            runs_addresses[current_address][2] = 0;         
            current_address++;
            running_rr_number++;
            index_neu = 0;
            flag_neu = false;
            if (rr_data[i-1] > rr_data[i]) {
                flag_acc = true;
            } else {
                flag_dec = true;
            }
        }
    }
    cout << "kulku!" << endl;
    for (int j = 0; j < current_address; j++) 
    {
        cout << j << endl;
        
        cout << " " << runs_addresses[j][0] 
             << " " << runs_addresses[j][1]
             << " " << runs_addresses[j][2] << endl;
    } 
    return runs;
}