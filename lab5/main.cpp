#include "CustomAllocator.hpp"
#include <vector>
#include <chrono>

int main(int argc, char const *argv[]) {

    std::vector<double, customAllocator<double>> custom;

    double startCustom = clock();

for (int i = 0; i < 1000; ++i) {
        custom.push_back(i);
    }

    double endCustom = clock();



    std::vector<double> standard;

    double startStandard = clock();

for (int i = 0; i < 1000; ++i) {
        standard.push_back(i + 0.1);
    }

    double endStandard = clock();

    std::cout << "Custom: " << endCustom - startCustom << "\nStandard: " << endStandard - startStandard << std::endl;

    return 0;
}

