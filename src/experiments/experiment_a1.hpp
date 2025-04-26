#ifndef SRC_EXPERIMENT_A1_H_
#define SRC_EXPERIMENT_A1_H_

#include <stdint.h>
#include <iostream>

//
// To avoid linking errors, implement the method in the header :(
//

template <typename T>
void experiment_A1(T start, T end) {
    T val = 0;
    uint64_t steps = 0;
    uint64_t ten_percent = (end - start) / 10;
    for (T i = start; i <= end; i++) {
        val = i;
        steps = 0;
        while(true) {
            if (val == 1) {
                break;
            }
            steps++;
            if(val % 2 == 0) {
                val /= 2;
            } else {
                val = 3 * val + 1;
            }
        }
        if (i % ten_percent == 0) {
            std::cout << "Finished another 10%: " << i << "/" << (end - start) << ".\n";
        }
    }
}

#endif
