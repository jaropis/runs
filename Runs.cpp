#include "Runs.h"
#include <iostream>
#include <fstream>

using namespace std;

RRRuns::RRRuns(std::vector<double> RR, std::vector<int> annot, bool write_last_run)
{
    this->rr_data = RR;
    this->annotations = annot;
    this->write_last_run = write_last_run;
    // declaring the accumulator -- all initialized to 0
    accumulator.dec.resize(rr_data.size());
    accumulator.acc.resize(rr_data.size());
    accumulator.neu.resize(rr_data.size());
}

void RRRuns::print_addresses(int how_many, RunType runType, int runLength)
{
    for (int j = 0; j < how_many; j++)
    {
        if (accumulator.runs_addresses[j][2] == runType && accumulator.runs_addresses[j][1] == runLength)
        {
            for (int i = 0; i < accumulator.runs_addresses[j][1]; i++)
            {
                cout << rr_data[accumulator.runs_addresses[j][0] + i] << " ";
            }
            cout << "\n";
        }
    }
}

void RRRuns::update_runs_addresses(vector<int> new_entry)
{
    this->accumulator.runs_addresses.push_back(new_entry);
}

void RRRuns::analyzeRuns()
{
    bool flag_dec = false;
    bool flag_acc = false;
    bool flag_neu = false;
    int index_dec = 0;
    int index_acc = 0;
    int index_neu = 0;
    int run_counter = 0;
    int running_rr_number = 0;
    int current_address = 0; // holds the address in runs_addresses array, adds consecutive runs

    // rewind to the first good flag
    while (annotations[running_rr_number] != 0 && running_rr_number < rr_data.size())
    {
        cout << "przejechalim" << endl;
        running_rr_number++;
    }
    running_rr_number++; // so that the initialization below can use -1
    // initializing the flags
    cout << "pierwszy i drugi element rr: " << rr_data[running_rr_number - 1] << " "
         << rr_data[running_rr_number] << endl;
    cout << "pierwszy i drugi element annot: " << annotations[running_rr_number - 1] << " "
         << annotations[running_rr_number] << endl;
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
        cout << "zaszlo!";
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
                    accumulator.acc[index_acc] += 1;
                    update_runs_addresses({running_rr_number, index_acc, RunType::ACC});
                    current_address++;
                    running_rr_number++;
                    index_acc = 0;
                    flag_acc = false;
                    flag_dec = true;
                }
                if (flag_neu)
                {
                    accumulator.neu[index_neu] += 1;
                    update_runs_addresses({running_rr_number, index_neu, RunType::NEU});
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
                        accumulator.dec[index_dec] += 1;
                        update_runs_addresses({running_rr_number, index_dec, RunType::DEC});
                        current_address++;
                        running_rr_number++;
                        index_dec = 0;
                        flag_dec = false;
                        flag_acc = true;
                    }
                    if (flag_neu)
                    {
                        accumulator.neu[index_neu] += 1;
                        update_runs_addresses({running_rr_number, index_neu, RunType::NEU});
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

                    accumulator.dec[index_dec] += 1;
                    update_runs_addresses({running_rr_number, index_dec, RunType::DEC});
                    current_address++;
                    running_rr_number++;
                    index_dec = 0;
                    flag_dec = false;
                    flag_neu = true;
                }
                if (flag_acc)
                {
                    accumulator.acc[index_acc] += 1;
                    update_runs_addresses({running_rr_number, index_acc, RunType::ACC});
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
            accumulator.acc[index_acc] += 1;
            update_runs_addresses({running_rr_number, index_acc, RunType::ACC});
        }
        if (index_dec > 0)
        {
            accumulator.dec[index_dec] += 1;
            update_runs_addresses({running_rr_number, index_dec, RunType::DEC});
        }
        if (index_neu > 0)
        {
            accumulator.neu[index_neu] += 1;
            update_runs_addresses({running_rr_number, index_neu, RunType::NEU});
        }
    }
    else
    {
        cout << "the last run not needed" << endl;
    }

    cout << "lacznie mamy: " << current_address << " serii" << endl;
    cout << "dlugosc szeregu to: " << rr_data.size() << endl;
    cout << "running_rr_number wyszedl: " << running_rr_number << endl;
    analyzed_ = true;
    /*for (int elem : accumulator.acc) {
        cout << elem << endl;
    }*/
}

RunsAccumulator RRRuns::getFullRuns()
{
    if (!analyzed_)
    {
        analyzeRuns();
    }
    return this->accumulator;
}

void RRRuns::print_runs()
{
    if (!analyzed_)
    {
        analyzeRuns();
    }
    int accSize = 20; // accumulator.dec.size();
    int decSize = 20; // accumulator.acc.size();
    int maxLength = std::max(accSize, decSize);
    cout << "i"
         << " Ar "
         << " - "
         << "DR"
         << " - "
         << "N" << "\n";
    for (int i = 1; i < maxLength; i++)
    {
        cout << i << " "
             << (i < accSize ? accumulator.acc[i] : 0)
             << " - "
             << (i < decSize ? accumulator.dec[i] : 0)
             << " - "
             << (i < decSize ? accumulator.neu[i] : 0)
             << "\n";
    }
}