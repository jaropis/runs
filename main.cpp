#include <cassert>
#include <iostream>
#include <fstream>
#include "Runs.h"
#include "Runs.cpp"
int main(void)
{
    RRRuns RR("adamek.csv", true);
    RR.get_runs();
}