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
    double currentRR;
    int currentFlag;
    while (rrfile >> currentRR >> currentFlag)
    {
        result.RR.push_back(currentRR);
        result.annot.push_back(currentFlag);
    }
    result.size = result.RR.size();
    return result;
};

int main(void)
{
    // RRseries RRSeries = readRR("adamek.csv");
    // RRRuns RR(RRSeries.RR, RRSeries.annot, true);
    // RR.getFullRuns();
    // RR.printRuns();
    // RR.printAddresses(RunType::NEU, 2, false);
    std::vector<double> RR1 = {1.0, 2.0, 3.0, 0.0, 1.0, 2.0, 3.0, 0.0};
    std::vector<int> annot1 = {0, 0, 1, 0, 0, 1, 0, 0};
    RRRuns RR(RR1, annot1, true);
    RR.printRuns();
    RR.printAddresses(RunType::DEC, 1, true);
    RR.printAddresses(RunType::ACC, 1, true);
}