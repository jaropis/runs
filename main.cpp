#include <cassert>
#include <iostream>
#include <fstream>
#include "Runs.h"
#include "Runs.cpp"
int main(void)
{
    RRRuns RR("RR_short.csv", true);
    RR.get_runs();
}