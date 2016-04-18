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
    loop_overhead = get_loop_overhead(SAMPLE_SIZE);
}

double CPUBenchmark::get_sampled_average(uint64_t *cycles, int size, double ratio)
{
    double sum;
    sort(cycles, cycles+size);
    for (int i=0; i < size*ratio; i++) {
        sum += cycles[i];
    }
    return sum/(size*ratio);
}

double CPUBenchmark::get_read_overhead() {
    uint64_t begin, end;
    for(int i = 0; i < SAMPLE_SIZE; i++) {
        begin = rdtsc();
        end = rdtsc();
        cycles[i] = end-begin;
    }
    return get_sampled_average(cycles);
}

double CPUBenchmark::get_loop_overhead(int times) {
    uint64_t begin, end;
    begin = rdtsc();
    for(int i = 0; i < times; i++) {}
    end = rdtsc();
    
    return ((end - begin) - read_overhead) / times;
}

double CPUBenchmark::get_procedure_call_overhead(int num_arguments){
    uint64_t begin, end;
    switch (num_arguments) {
        case 0:
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                begin = rdtsc();
                fun_0();
                end = rdtsc();
                cycles[i] = end-begin;
            }
            break;
        case 1:
            
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                begin = rdtsc();    
                fun_1(0);
                end = rdtsc();
                cycles[i] = end-begin;
            }
            break;
        case 2:
            
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                begin = rdtsc();    
                fun_2(0, 0);
                end = rdtsc();
                cycles[i] = end-begin;
            }
            break;
        case 3:
            
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                begin = rdtsc();    
                fun_3(0, 0, 0);
                end = rdtsc();
                cycles[i] = end-begin;
            }
            break;
        case 4:
            
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                begin = rdtsc();    
                fun_4(0, 0, 0, 0);
                end = rdtsc();
                cycles[i] = end-begin;
            }
            break;
        case 5:
            
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                begin = rdtsc();    
                fun_5(0, 0, 0, 0, 0);
                end = rdtsc();
                cycles[i] = end-begin;
            }
            break;
        case 6:
            
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                begin = rdtsc();    
                fun_6(0, 0, 0, 0, 0, 0);
                end = rdtsc();
                cycles[i] = end-begin;
            }
            break;
        case 7:
            
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                begin = rdtsc();    
                fun_7(0, 0, 0, 0, 0, 0, 0);
                end = rdtsc();
                cycles[i] = end-begin;
            }
            break;
        default:
            // unanticipated input
            return -1;
            break;
    }
    return get_sampled_average(cycles) - read_overhead;
}

double CPUBenchmark::get_system_call_overhead(){
    /*
     http://man7.org/linux/man-pages/man2/getpid.2.html
     getpid() returns the process ID of the calling process.  (This is
     often used by routines that generate unique temporary filenames.)
     */
    uint64_t begin, end;

    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        begin = rdtsc();
        getpid();
        end = rdtsc();
        cycles[i] = end - begin;
    }
    
    return get_sampled_average(cycles) - read_overhead;
}