//
//  benchmark.cpp
//  Memory_measurment
//
//  Created by Kun on 5/2/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include "benchmark.h"

double MemoryBenchmark::get_latency(uint64_t array_size, uint64_t stride_size){
    int *array = (int*)malloc(array_size*sizeof(int));
    memset(array, 0, array_size*sizeof(*array));
    
    int tmp;
    uint64_t begin, end;
    
    begin = rdtsc();
    for (uint64_t index=0; index < array_size; index+=stride_size) {
        tmp = array[index];
    }
    end = rdtsc();
    
    free(array);
    return float(end - begin)/(array_size/stride_size);
}