#include "Runs.h"
#include <iostream>
#include <fstream>

using namespace std;

RRRuns::RRRuns(string path, bool write_last_run)
{
    ifstream rrfile;
    rrfile.open(path);
    double current_rr;
    int current_flag;
    this->write_last_run = write_last_run;
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
    int running_rr_number = 1;
    int current_address = 0; // holds the address in runs_addresses array, adds consecutive runs
    int accumulator_dec [rr_data.size()]; // accumulates statistics for acceleration runs
    int accumulator_acc [rr_data.size()]; // accumulates statistics for deceleration runs
    int accumulator_neu [rr_data.size()]; // accumulates statistics for deceleration runs

    // rewind to the first good flag
    while (annotations[running_rr_number - 1] != 0) {
        cout << "przejechalim" << endl;
        running_rr_number++;
    }
    running_rr_number++; // so that the initialization below can use -1
    // initializing the flags
    if (rr_data[running_rr_number - 1] < rr_data[running_rr_number]) 
    {
        flag_dec = true;
        index_dec++;
    }
    if (rr_data[running_rr_number - 1] > rr_data[running_rr_number]) 
    {
        flag_acc = true;
        index_acc++;
    }
    if (rr_data[running_rr_number - 1] == rr_data[running_rr_number])
    {
        flag_neu = true;
        index_neu++;
    }

    // TUTU!!! start thinking here!!! do we need the +2?
    running_rr_number += 2;
    // starting from the second RR interval
    running_rr_number += 1;
    for (int i=running_rr_number; i < rr_data.size(); i++) 
    {  
        // update running_rr_number (we start from +2 above)

        // what happens if annotation is not 0? We reset the flags and re-initiate the calculations
        if (annotations[i-1] != 0) {
            index_dec = 0;
            index_acc = 0;
            index_neu = 0;
            // rewind to the first good rr_(n-1)
            while (annotations[running_rr_number - 1] != 0) {
                cout << "przejechalim" << endl;
                running_rr_number++;
                i++;
            }
            // reinitializing the flags
            if (rr_data[running_rr_number - 1] < rr_data[running_rr_number]) 
            {
                flag_dec = true;
                index_dec++;
            }
            if (rr_data[running_rr_number - 1] > rr_data[running_rr_number]) 
            {
                flag_acc = true;
                index_acc++;
            }
            if (rr_data[running_rr_number - 1] == rr_data[running_rr_number])
            {
                flag_neu = true;
                index_neu++;
            }   
            continue; // and leave the main loop
        }

        if(rr_data[i-1] < rr_data[i])
        {   
            index_dec++;
            if (flag_dec) {
                running_rr_number++;
            } else {
                if (flag_acc) 
                {
                    accumulator_acc[index_acc] += 1;
                    runs_addresses[current_address][0] = running_rr_number;
                    runs_addresses[current_address][1] = index_acc;
                    runs_addresses[current_address][2] = -1;
                    current_address++;
                    running_rr_number++;
                    index_acc = 0;
                    flag_acc = false;
                    flag_dec = true;
                }
                if (flag_neu) 
                {
                    accumulator_neu[index_neu] += 1;
                    runs_addresses[current_address][0] = running_rr_number;
                    runs_addresses[current_address][1] = index_neu;         
                    runs_addresses[current_address][2] = 0;         
                    current_address++;
                    running_rr_number++;
                    index_neu = 0;
                    flag_neu = false;
                    flag_dec = true;
                }
            }              
        }
        if(rr_data[i-1] > rr_data[i]) {
        {   
            index_acc++;
            if (flag_acc)
            {
                running_rr_number++;
            } else {
                if (flag_dec) 
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
                if (flag_neu)
                {
                    accumulator_neu[index_neu] += 1;
                    runs_addresses[current_address][0] = running_rr_number;
                    runs_addresses[current_address][1] = index_neu;         
                    runs_addresses[current_address][2] = 0;         
                    current_address++;
                    running_rr_number++;
                    index_neu = 0;
                    flag_neu = false;
                    flag_acc = true;
                }
            }
        }
        }
        if(rr_data[i-1] == rr_data[i])
        {
            index_neu++;
            if (flag_neu) 
            {   
                running_rr_number++;
            } else {
                if (flag_dec) 
                {
                    
                    accumulator_dec[index_dec] += 1;
                    runs_addresses[current_address][0] = running_rr_number;
                    runs_addresses[current_address][1] = index_dec;
                    runs_addresses[current_address][2] = 1;
                    current_address++;
                    running_rr_number++;
                    index_dec = 0;     
                    flag_dec = false;
                    flag_neu = true; 
                }
                if (flag_acc) 
                {   
                    accumulator_acc[index_acc] += 1;
                    runs_addresses[current_address][0] = running_rr_number;
                    runs_addresses[current_address][1] = index_acc;
                    runs_addresses[current_address][2] = -1;
                    current_address++;
                    running_rr_number++;
                    index_acc = 0;
                    flag_acc = false;
                    flag_neu = true;
                }
            }
            
        }
    }

    // writing the last run
    if (this->write_last_run) {
        if (index_acc > 0) {
            accumulator_acc[index_acc] += 1;
            runs_addresses[current_address][0] = running_rr_number;
            runs_addresses[current_address][1] = index_acc;
            runs_addresses[current_address][2] = -1;
        }
        if (index_dec > 0) {
            accumulator_dec[index_dec] += 1;
            runs_addresses[current_address][0] = running_rr_number;
            runs_addresses[current_address][1] = index_dec;
            runs_addresses[current_address][2] = 1;
        }
        if (index_neu > 0) {
            accumulator_neu[index_neu] += 1;
            runs_addresses[current_address][0] = running_rr_number;
            runs_addresses[current_address][1] = index_neu;         
            runs_addresses[current_address][2] = 0;
        }
    } else {
        cout << "the last run not needed" << endl;
    }
    for (int j = 0; j <= current_address; j++) 
    {   
        cout << " " << runs_addresses[j][0] 
             << " " << runs_addresses[j][1]
             << " " << runs_addresses[j][2] << endl;
    } 
    cout << "lacznie mamy: " << current_address << " serii" << endl;
    cout << "dlugosc szeregu to: " << rr_data.size() << endl;
    cout << "running_rr_number wyszedl: " << running_rr_number << endl;
    /*for (int elem : accumulator_acc) {
        cout << elem << endl;
    }*/
    return runs;
}