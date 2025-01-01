#include "Runs.h"
#include <iostream>
#include <fstream>

using namespace std;

RRRuns::RRRuns(string path, bool write_last_run)
{
    ifstream rrfile;
    rrfile.open(path);
    if (!rrfile.is_open())
    {
        cout << "Error opening file " << path;
    };
    double current_rr;
    int current_flag;
    while (rrfile >> current_rr >> current_flag)
    {
        this->rr_data.push_back(current_rr);
        this->annotations.push_back(current_flag);
    }
    this->write_last_run = write_last_run;
}

void RRRuns::print_addresses(int how_many, int max_length)
{
    how_many = (how_many > max_length) ? max_length : how_many;
    for (int j = 0; j < how_many; j++)
    {
        cout << "dla j: " << j << endl;
        cout << runs_addresses[j][0] << " " << runs_addresses[j][1] << " " << runs_addresses[j][2] << endl;
    }
}

void RRRuns::update_runs_addresses(vector<int> new_entry)
{
    this->runs_addresses.push_back(new_entry);
}

vector<int> RRRuns::get_runs()
{
    bool flag_dec = false;
    bool flag_acc = false;
    bool flag_neu = false;
    int index_dec = 0;
    int index_acc = 0;
    int index_neu = 0;
    int run_counter = 0;
    int running_rr_number = 0;
    int current_address = 0;                             // holds the address in runs_addresses array, adds consecutive runs
    std::vector<int> accumulator_dec(rr_data.size(), 0); // accumulates statistics for acceleration runs
    std::vector<int> accumulator_acc(rr_data.size(), 0); // accumulates statistics for deceleration runs
    std::vector<int> accumulator_neu(rr_data.size(), 0); // accumulates statistics for neutral runs
    // rewind to the first good flag
    while (annotations[running_rr_number] != 0)
    {
        cout << "przejechalim" << endl;
        running_rr_number++;
    }
    running_rr_number++; // so that the initialization below can use -1
    // initializing the flags
    cout << "pierwszy i drugi element: " << rr_data[running_rr_number - 1] << " "
         << rr_data[running_rr_number] << endl;
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

    for (int i = running_rr_number; i < rr_data.size(); i++)
    {
        // update running_rr_number (we start from +2 above)

        // what happens if annotation is not 0? We reset the flags and re-initiate the calculations
        if (annotations[i - 1] != 0)
        {
            index_dec = 0;
            index_acc = 0;
            index_neu = 0;
            // rewind to the first good rr_(n-1)
            while (annotations[running_rr_number - 1] != 0)
            {
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
        // if as a result of skipping over non sinus beats we are over the length of the rr intervals time series, break out of the loop
        if (i >= rr_data.size())
        {
            break;
        }

        if (rr_data[i - 1] < rr_data[i])
        {
            index_dec++;
            if (flag_dec)
            {
                running_rr_number++;
            }
            else
            {
                if (flag_acc)
                {
                    accumulator_acc[index_acc] += 1;
                    update_runs_addresses({running_rr_number, index_acc, -1});
                    current_address++;
                    running_rr_number++;
                    index_acc = 0;
                    flag_acc = false;
                    flag_dec = true;
                }
                if (flag_neu)
                {
                    accumulator_neu[index_neu] += 1;
                    update_runs_addresses({running_rr_number, index_neu, 0});
                    current_address++;
                    running_rr_number++;
                    index_neu = 0;
                    flag_neu = false;
                    flag_dec = true;
                }
            }
        }
        if (rr_data[i - 1] > rr_data[i])
        {
            {
                index_acc++;
                if (flag_acc)
                {
                    running_rr_number++;
                }
                else
                {
                    if (flag_dec)
                    {
                        accumulator_dec[index_dec] += 1;
                        update_runs_addresses({running_rr_number, index_dec, 1});
                        current_address++;
                        running_rr_number++;
                        index_dec = 0;
                        flag_dec = false;
                        flag_acc = true;
                    }
                    if (flag_neu)
                    {
                        accumulator_neu[index_neu] += 1;
                        update_runs_addresses({running_rr_number, index_neu, 0});
                        current_address++;
                        running_rr_number++;
                        index_neu = 0;
                        flag_neu = false;
                        flag_acc = true;
                    }
                }
            }
        }
        if (rr_data[i - 1] == rr_data[i])
        {
            index_neu++;
            if (flag_neu)
            {
                running_rr_number++;
            }
            else
            {
                if (flag_dec)
                {

                    accumulator_dec[index_dec] += 1;
                    update_runs_addresses({running_rr_number, index_dec, 1});
                    current_address++;
                    running_rr_number++;
                    index_dec = 0;
                    flag_dec = false;
                    flag_neu = true;
                }
                if (flag_acc)
                {
                    accumulator_acc[index_acc] += 1;
                    update_runs_addresses({running_rr_number, index_acc, -1});
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
    if (this->write_last_run)
    {
        if (index_acc > 0)
        {
            accumulator_acc[index_acc] += 1;
            update_runs_addresses({running_rr_number, index_acc, -1});
        }
        if (index_dec > 0)
        {
            accumulator_dec[index_dec] += 1;
            update_runs_addresses({running_rr_number, index_dec, 1});
        }
        if (index_neu > 0)
        {
            accumulator_neu[index_neu] += 1;
            update_runs_addresses({running_rr_number, index_neu, 0});
        }
    }
    else
    {
        cout << "the last run not needed" << endl;
    }

    full_runs["dec"] = accumulator_dec;
    full_runs["acc"] = accumulator_acc;
    full_runs["neu"] = accumulator_neu;
    print_addresses(10, current_address);

    cout << "lacznie mamy: " << current_address << " serii" << endl;
    cout << "dlugosc szeregu to: " << rr_data.size() << endl;
    cout << "running_rr_number wyszedl: " << running_rr_number << endl;
    /*for (int elem : accumulator_acc) {
        cout << elem << endl;
    }*/
    return runs;
}