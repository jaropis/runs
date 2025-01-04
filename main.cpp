#include <cassert>
#include <iostream>
#include <fstream>
#include "Runs.h"
#include <string>
#include <vector>
struct RRseries
{
    std::vector<double> RR;
    std::vector<int> annot;
    int size;
};

RRseries readRR(std::string path)
{
    RRseries result;
    std::ifstream rrfile;
    rrfile.open(path);
    if (!rrfile.is_open())
    {
        std::cout << "Error opening file " << path;
    };
    double current_rr;
    int current_flag;
    while (rrfile >> current_rr >> current_flag)
    {
        result.RR.push_back(current_rr);
        result.annot.push_back(current_flag);
    }
    result.size = result.RR.size();
    return result;
};

int main(void)
{
    RRseries RRSeries = readRR("adamek.csv");
    RRRuns RR(RRSeries.RR, RRSeries.annot, false);
    RR.getFullRuns();
    RR.print_runs();
    RR.print_addresses(RRSeries.size, RunType::NEU, 2);
}