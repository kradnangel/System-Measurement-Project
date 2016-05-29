//
//  benchmark.cpp
//  Memory_measurment
//
//  Created by Kun on 5/2/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include "benchmark.h"
void MemoryBenchmark::flush_cache(){
    void **array = (void**)malloc(FLUSH_SIZE*sizeof(void *));
    memset(array, 0, FLUSH_SIZE*sizeof(void *));
    free(array);
}

double MemoryBenchmark::get_latency(uint64_t array_size, uint64_t stride_size){
    uint64_t count = array_size/stride_size-1;
    if (count > MAXIMUM_COUNT || count != 0)
        return -1.;
    
    void **array = (void**)malloc(array_size*sizeof(void *));
    memset(array, 0, array_size*sizeof(*array));
    
    for (int index=0; index < array_size; index+=stride_size) {
        array[index] = (void*)&(array[index+stride_size]);
    }

    uint64_t begin, end;
    void **p;
    
//    flush_cache();
//    p = array;
//    for (int j = 0; j < count; j++) {
//        p = (void**)*p;
//    }
    
    begin = rdtsc();
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        p = array;
        for (int j = 0; j < count; j++) {
            p = (void**)*p;
        }
    }
    end = rdtsc();
    
    free(array);
    return float(end - begin)/(SAMPLE_SIZE*count);
}

//no random, no 10 base
double MemoryBenchmark::latency1 (uint64_t  arraySize, uint64_t  strideSize) {
    double start;
    double end;
    double rawTime;
    //check the word, hence use char pointer
    char ** p_list = new char* [arraySize];
    for ( int i =0; i < arraySize ; i ++) {
        uint64_t  index =  (i/strideSize * strideSize + strideSize)%arraySize;
        p_list [i] = (char * ) &p_list[index];     // pointer at i point to index
    }
    
    char ** p = p_list;
    
    uint64_t Count = arraySize/ (strideSize ) + 1;
    //why base 10 , because check bound each time cost a lot of time
    start = rdtsc();
    for ( int i = 0 ; i < SAMPLE_SIZE; i ++ ) {
        for ( int j = 0; j < Count ; j ++) {
            
            p = (char**) *p;
            // check ++;
            //   fakePointer = p;
            
        }
        // p = p_list;
    }
    end = rdtsc();
    rawTime = (end - start);
    
    // if (!fakePointer) std::cout<<std::endl<<"no!!!!!!!!! compiler optimize your code!!!!!!!!"<<std::endl;
    
    // std::cout <<"check" << check ;
    double time = (double)rawTime / (SAMPLE_SIZE *Count);
    delete [] p_list;
    
    return time;
}

double  MemoryBenchmark::get_write_bandwidth(uint64_t array_size){
    char *array = (char *)malloc(array_size*sizeof(char));
    uint64_t begin, end;
        
    memset(array, 0, array_size);

    begin = rdtsc();
    for (int j=0; j < SAMPLE_SIZE; j++) {
        for (int i=0; i < array_size; i+= CACHE_LINE) {
            array[i] = 0;
        }
    }
    end = rdtsc();
    
    free(array);
    // GB/s
    return double(array_size) * SAMPLE_SIZE / (double(end-begin) / 2.4 * GB * NANOSECOND) ;
}

double  MemoryBenchmark::get_read_bandwidth(uint64_t array_size){
    char *array = (char *)malloc(array_size*sizeof(char));
    uint64_t begin, end;
    
    memset(array, 0, array_size);
    
    char tmp;
    begin = rdtsc();
    for (int j=0; j < SAMPLE_SIZE; j++) {
        for (int i=0; i < array_size; i+= CACHE_LINE) {
            tmp = array[i];
        }
    }
    end = rdtsc();
    
    free(array);
    // GB/s
    return double(array_size) * SAMPLE_SIZE / (double(end-begin) / 2.4 * GB * NANOSECOND) ;
}