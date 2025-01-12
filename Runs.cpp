#include "Runs.h"
#include <iostream>
#include <fstream>

using namespace std;
RRRuns::RRRuns(std::vector<double> RR, std::vector<int> annot, bool writeLastRun) : rrIntervals(std::move(RR)), annotations(std::move(annot)), writeLastRun(writeLastRun)
{
    // declaring the accumulator -- all initialized to 0
    accumulator.dec.resize(rrIntervals.size());
    accumulator.acc.resize(rrIntervals.size());
    accumulator.neu.resize(rrIntervals.size());
}

void RRRuns::printAddresses(RunType runType, int runLength, bool referenceBeat)
{
    if (!analyzed_)
    {
        analyzeRuns();
    }
    int referenceOffset = referenceBeat ? 1 : 0;

    for (int j = 0; j < accumulator.runs_addresses.size(); j++)
    {
        if (accumulator.runs_addresses[j][2] == runType && accumulator.runs_addresses[j][1] == runLength)
        {
            // remembering that the runs address is the address of the last beat belonging to a run
            for (int i = -(accumulator.runs_addresses[j][1] + referenceOffset); i < 0; i++) // + 1, because we want also to see the reference beat
            {
                cout << rrIntervals[accumulator.runs_addresses[j][0] + i] << " ";
            }
            cout << "\n";
        }
    }
}

void RRRuns::updateRunsAddresses(vector<int> new_entry)
{
    this->accumulator.runs_addresses.push_back(new_entry);
}

void RRRuns::analyzeRuns()
{
    bool flagDec = false;
    bool flagAcc = false;
    bool flagNeu = false;
    int indexDec = 0;
    int indexAcc = 0;
    int indexNeu = 0;
    int runCounter = 0;
    int runningRRNumber = 0;
    int currentAddress = 0; ///> holds the address in runs_addresses array, adds consecutive runs

    /// rewind to the first good flag
    while (annotations[runningRRNumber] != 0 && runningRRNumber < rrIntervals.size())
    {
        runningRRNumber++;
    }
    // /initializing the flags
    runningRRNumber++; ///> only for stylistic reasons: so that I can use runningRRNumber - 1 below
    if (rrIntervals[runningRRNumber - 1] < rrIntervals[runningRRNumber])
    {
        flagDec = true;
        indexDec++;
    }
    if (rrIntervals[runningRRNumber - 1] > rrIntervals[runningRRNumber])
    {
        flagAcc = true;
        indexAcc++;
    }
    if (rrIntervals[runningRRNumber - 1] == rrIntervals[runningRRNumber])
    {
        flagNeu = true;
        indexNeu++;
    }
    for (int i = runningRRNumber + 1; i < rrIntervals.size(); i++) // + 2, in order to be able to use runningRRNumber - 1 later
    {
        // update runningRRNumber (we start from +2 above)

        // what happens if annotation is not 0? We reset the flags and re-initiate the calculations
        if (annotations[i - 1] != 0)
        {
            indexDec = 0;
            indexAcc = 0;
            indexNeu = 0;
            // rewind to the first good rr_(n-1)
            while (annotations[runningRRNumber - 1] != 0)
            {
                runningRRNumber++;
                i++;
            }
            // reinitializing the flags
            if (rrIntervals[runningRRNumber - 1] < rrIntervals[runningRRNumber])
            {
                flagDec = true;
                indexDec++;
            }
            if (rrIntervals[runningRRNumber - 1] > rrIntervals[runningRRNumber])
            {
                flagAcc = true;
                indexAcc++;
            }
            if (rrIntervals[runningRRNumber - 1] == rrIntervals[runningRRNumber])
            {
                flagNeu = true;
                indexNeu++;
            }
            continue; // and leave the main loop
        }
        // if as a result of skipping over non sinus beats we are over the length of the rr intervals time series, break out of the loop
        if (i >= rrIntervals.size())
        {
            break;
        }

        if (rrIntervals[i - 1] < rrIntervals[i])
        {
            indexDec++;
            if (flagDec)
            {
                runningRRNumber++;
            }
            else
            {
                if (flagAcc)
                {
                    accumulator.acc[indexAcc] += 1;
                    updateRunsAddresses({runningRRNumber, indexAcc, RunType::ACC});
                    currentAddress++;
                    runningRRNumber++;
                    indexAcc = 0;
                    flagAcc = false;
                    flagDec = true;
                }
                if (flagNeu)
                {
                    accumulator.neu[indexNeu] += 1;
                    updateRunsAddresses({runningRRNumber, indexNeu, RunType::NEU});
                    currentAddress++;
                    runningRRNumber++;
                    indexNeu = 0;
                    flagNeu = false;
                    flagDec = true;
                }
            }
        }
        if (rrIntervals[i - 1] > rrIntervals[i])
        {
            {
                indexAcc++;
                if (flagAcc)
                {
                    runningRRNumber++;
                }
                else
                {
                    if (flagDec)
                    {
                        accumulator.dec[indexDec] += 1;
                        updateRunsAddresses({runningRRNumber, indexDec, RunType::DEC});
                        currentAddress++;
                        runningRRNumber++;
                        indexDec = 0;
                        flagDec = false;
                        flagAcc = true;
                    }
                    if (flagNeu)
                    {
                        accumulator.neu[indexNeu] += 1;
                        updateRunsAddresses({runningRRNumber, indexNeu, RunType::NEU});
                        currentAddress++;
                        runningRRNumber++;
                        indexNeu = 0;
                        flagNeu = false;
                        flagAcc = true;
                    }
                }
            }
        }
        if (rrIntervals[i - 1] == rrIntervals[i])
        {
            indexNeu++;
            if (flagNeu)
            {
                runningRRNumber++;
            }
            else
            {
                if (flagDec)
                {

                    accumulator.dec[indexDec] += 1;
                    updateRunsAddresses({runningRRNumber, indexDec, RunType::DEC});
                    currentAddress++;
                    runningRRNumber++;
                    indexDec = 0;
                    flagDec = false;
                    flagNeu = true;
                }
                if (flagAcc)
                {
                    accumulator.acc[indexAcc] += 1;
                    updateRunsAddresses({runningRRNumber, indexAcc, RunType::ACC});
                    currentAddress++;
                    runningRRNumber++;
                    indexAcc = 0;
                    flagAcc = false;
                    flagNeu = true;
                }
            }
        }
    }

    // writing the last run
    if (this->writeLastRun)
    {
        if (indexAcc > 0)
        {
            accumulator.acc[indexAcc] += 1;
            updateRunsAddresses({runningRRNumber, indexAcc, RunType::ACC});
        }
        if (indexDec > 0)
        {
            accumulator.dec[indexDec] += 1;
            updateRunsAddresses({runningRRNumber, indexDec, RunType::DEC});
        }
        if (indexNeu > 0)
        {
            accumulator.neu[indexNeu] += 1;
            updateRunsAddresses({runningRRNumber, indexNeu, RunType::NEU});
        }
    }
    else
    {
        cout << "the last run not needed" << endl;
    }

    cout << "lacznie mamy: " << currentAddress << " serii" << endl;
    cout << "dlugosc szeregu to: " << rrIntervals.size() << endl;
    cout << "runningRRNumber wyszedl: " << runningRRNumber << endl;
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

void RRRuns::printRuns()
{
    if (!analyzed_)
    {
        analyzeRuns();
    }
    int decSize = getNonzeroLength(accumulator.dec);
    int accSize = getNonzeroLength(accumulator.acc);
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

int RRRuns::getNonzeroLength(vector<int> vec)
{
    int counter = vec.size();
    int i;
    for (i = counter; i > 0; i--)
    {
        if (vec[i] != 0)
        {
            break;
        }
    }
    return i + 1; // because we actually index from 1
}