//
//  benchmark.cpp
//  CPU_measurement
//
//  Created by Kun on 4/17/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include "benchmark.h"
#include <ctime>

#define TIMES 100000

using namespace std;

double CPUBenchmark::get_read_overhead() {
    double sum = 0;
    
    for(int i = 0; i < TIMES; i++) {
        clock_t begin = clock();
        clock_t end = clock();
        sum += double(end - begin) / CLOCKS_PER_SEC;
    }
    
    return (double)sum / (double)TIMES;
}

double CPUBenchmark::get_loop_overhead(int times) {
    clock_t begin = clock();
    for(int i = 0; i < times; i++) {}
    clock_t end = clock();
    
    return double(end - begin) / CLOCKS_PER_SEC;
}