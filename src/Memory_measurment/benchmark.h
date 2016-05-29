//
//  benchmark.h
//  Memory_measurment
//
//  Created by Kun on 5/2/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#ifndef Memory_measurment_benchmark_h
#define Memory_measurment_benchmark_h

#define SAMPLE_SIZE 100
#define MAXIMUM_COUNT (1<<16)
#define FLUSH_SIZE (1<<23)
#define CYCLE_PER_SEC 2400000000
#define CACHE_LINE 64
#define GB (1<<30)
#define NANOSECOND 0.000000001

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <pthread.h>
#include <string.h>
#include <algorithm>

class MemoryBenchmark{
    
private:
    void flush_cache();
public:
    double get_latency(uint64_t array_size, uint64_t stride_size);
    double latency1 (uint64_t  arraySize, uint64_t  strideSize);
    double get_write_bandwidth(uint64_t array_size);
    double get_read_bandwidth(uint64_t array_size);
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
