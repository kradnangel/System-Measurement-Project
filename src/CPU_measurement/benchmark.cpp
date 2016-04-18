//
//  benchmark.cpp
//  CPU_measurement
//
//  Created by Kun on 4/17/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include "benchmark.h"
#include <ctime>


using namespace std;

CPUBenchmark::CPUBenchmark() {
    read_overhead = get_read_overhead();
    loop_overhead = get_loop_overhead(TIMES);
}

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
    
    return (double(end - begin)/CLOCKS_PER_SEC - read_overhead) / times;
}

double CPUBenchmark::get_procedure_call_overhead(int num_arguments){
    clock_t begin, end;
    switch (num_arguments) {
        case 0:
            begin = clock();
            for (int i = 0; i < TIMES; i++)
                fun_0();
            end = clock();
            break;
        case 1:
            begin = clock();
            for (int i = 0; i < TIMES; i++)
                fun_1(0);
            end = clock();
            break;
        case 2:
            begin = clock();
            for (int i = 0; i < TIMES; i++)
                fun_2(0, 0);
            end = clock();
            break;
        case 3:
            begin = clock();
            for (int i = 0; i < TIMES; i++)
                fun_3(0, 0, 0);
            end = clock();
            break;
        case 4:
            begin = clock();
            for (int i = 0; i < TIMES; i++)
                fun_4(0, 0, 0, 0);
            end = clock();
            break;
        case 5:
            begin = clock();
            for (int i = 0; i < TIMES; i++)
                fun_5(0, 0, 0, 0, 0);
            end = clock();
            break;
        case 6:
            begin = clock();
            for (int i = 0; i < TIMES; i++)
                fun_6(0, 0, 0, 0, 0, 0);
            end = clock();
            break;
        case 7:
            begin = clock();
            for (int i = 0; i < TIMES; i++)
                fun_7(0, 0, 0, 0, 0, 0, 0);
            end = clock();
            break;
        default:
            // unanticipated input
            return -1;
            break;
    }
    return (double(end - begin)/CLOCKS_PER_SEC - read_overhead) / TIMES - loop_overhead;
}