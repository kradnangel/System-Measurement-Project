//
//  benchmark.h
//  Memory_measurment
//
//  Created by Kun on 5/2/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#ifndef Memory_measurment_benchmark_h
#define Memory_measurment_benchmark_h

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <pthread.h>
#include <string.h>

class MemoryBenchmark{
    
public:
    double get_latency(uint64_t array_size, uint64_t stride_size);
};

static inline uint64_t rdtsc(){
    //    unsigned int lo,hi;
    //    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    //    return ((uint64_t)hi << 32) | lo;
    
    uint64_t tsc;
    __asm__ __volatile__("rdtscp; "         // serializing read of tsc
                         "shl $32,%%rdx; "  // shift higher 32 bits stored in rdx up
                         "or %%rdx,%%rax"   // and or onto rax
                         : "=a"(tsc)        // output to tsc variable
                         :
                         : "%rcx", "%rdx"); // rcx and rdx are clobbered
    return tsc;
}

#endif
