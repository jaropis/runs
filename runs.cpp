#include <cassert>
#include <iostream>
#include <fstream>
int main(void)
{
    int RR_length = 1943;
    int runs_length = 300;
    int i = 0;
    double RR[2][RR_length];
    double a, b;
    std::ifstream read_file("RR1.csv");
    assert(read_file.is_open());

    while (!read_file.eof())
    // while (i < 8)
    {
        // std::cout << read_file;
        read_file >> RR[1][i] >> RR[2][i];
        std::cout << RR[1][i] << " " << RR[2][i] << std::endl;
        i++;
    }
    read_file.close();

    /* for (int i = 0; i < 100; i++)
    {
        std::cout << RR[1][i] << " " << RR[2][i] << std::endl;
    } */
}