#include <cassert>
#include <iostream>
#include <fstream>
#include "Runs.h"

int main(void)
{
    RRRuns RR("adamek.csv", true);
    RR.get_runs();
}