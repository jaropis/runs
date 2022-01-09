#include <cassert>
#include <iostream>
#include <fstream>
int main(int argc, char *argv[])
{
    double x[6], y[6];
    std::ifstream read_file("OutputFormatted.dat");
    assert(read_file.is_open());
    for (int i = 0; i < 6; i++)
    {
        // std::cout << read_file << std::endl;
        read_file >> x[i] >> y[i];
        std::cout << x[i] << " " << y[i] << std::endl;
    }
    read_file.close();
    return 0;
}
