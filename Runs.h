#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

enum RunType
{
    DEC = 1,
    NEU = 0,
    ACC = -1
};

struct RunsAccumulator
{
    std::vector<int> dec;                         // accumulates statistics for acceleration runs
    std::vector<int> acc;                         // accumulates statistics for deceleration runs
    std::vector<int> neu;                         // accumulates statistics for neutral runs
    std::vector<std::vector<int>> runs_addresses; // this holds addresses of decelerations - the first row contains the address of the end, the second the length of the run, and the third the type of run (-1 is acc, 0 is neutral, 1 is dec)
};

class RRRuns
{
public:
    RRRuns(std::vector<double> RR, std::vector<int> annot, bool write_last_run);
    std::vector<int> runs;
    RunsAccumulator getFullRuns();
    void print_runs();
    void print_addresses(RunType runType, int runLength, bool referenceBeat);

private:
    std::vector<double> rr_data;
    std::vector<int> annotations;
    bool write_last_run;
    void update_runs_addresses(std::vector<int> new_entry);
    RunsAccumulator accumulator;
    void analyzeRuns();
    bool analyzed_ = false;
    // vector<int> rollup_runs(int accumulator_dec[], int accumulator_acc[], int accumulator_neu[]);
};