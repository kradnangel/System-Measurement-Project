//
//  main.cpp
//  Memory_measurment
//
//  Created by Kun on 5/2/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include <iostream>
#include "benchmark.h"

uint64_t array_size_list[] = {0x400, 0x8000, 0x100000, 0x2000000};
uint64_t stride_list[] = {0x1, 0x10, 0x400};


using namespace std;

string format_size(uint64_t size){
    if (size < 0x400) {
        return to_string(size);
    }
    else if (size < 0x100000) {
        return to_string(size/0x400)+'K';
    }
    else if (size < 0x40000000) {
        return to_string(size/0x100000)+'M';
    }
    else {
        return to_string(size/0x40000000)+'G';
    }
}
int main(int argc, const char * argv[])
{
    MemoryBenchmark benchmark;
    
    /* 1. Measurement overhead */
    cout << "1. RAM access time" << endl;
    for (int i=0; i < sizeof(array_size_list)/sizeof(*array_size_list); i++) {
        for (int j=0; j < sizeof(stride_list)/sizeof(*stride_list); j++) {
            if(stride_list[j] < array_size_list[i]){
                cout << "Array Size: " << format_size(array_size_list[i]) << ", ";
                cout << "Stride: " << stride_list[j] << ", ";
                cout << "Latency: " << benchmark.get_latency(array_size_list[i], stride_list[j]) << endl;
            }
        }
    }
    
    return 0;
}