#include <stdint.h>
#include <inttypes.h>
#include <iostream>
#include "experiments/experiment_a1.hpp"


int main() {

    // We need an "explain" method for any given number, printed out in a neat graph.

    uint64_t start = 1;
    uint64_t end = 100000000000;
    std::cout << "Starting test from " << start << " to " << end << ".\n";
    experiment_A1(start, end);
    std::cout << "Done.\n";

    return 0;
}
