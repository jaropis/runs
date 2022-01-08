#include <cassert>
#include <iostream>
#include <fstream>
int main(void)
{
    int RR_length = 1943;
    int runs_length = 300;

    double RR[2][RR_length];
    std::ifstream read_file("RR.csv");
    assert(read_file.is_open());
    for (int i = 0; i < RR_length; i++)
    {
        std::cout << read_file;
        /* read_file >>
            RR[1][i] >> RR[2][i]; */
    }
    read_file.close();

    /* for (int i = 0; i < 100; i++)
    {
        std::cout << RR[1][i] << " " << RR[2][i] << std::endl;
    } */
}