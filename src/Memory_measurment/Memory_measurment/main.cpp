//
//  main.cpp
//  Memory_measurment
//
//  Created by Kun on 5/2/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include <iostream>
#include "benchmark.h"
#include <fstream>

uint64_t array_size_list[] = {1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18, 1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24, 1 << 25, 1 << 26, 1 << 27, 1 << 28};
uint64_t stride_list[] = {1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12};

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
    
    /* 1. RAM access time */
    /*
    cout << "1. RAM access time" << endl;
    cout << "writing to " << RAM_output_name << endl;
     
    ofstream RAM_output;
    string RAM_output_name = "RAM_access_time.csv";
    RAM_output.open(RAM_output_name);
     
    RAM_output << "Array Size, ";
    for (int j=0; j < sizeof(stride_list)/sizeof(*stride_list); j++) {
        RAM_output << "Stride=" << stride_list[j] << ", ";
    }
    RAM_output << endl;
    
    for (int i=0; i < sizeof(array_size_list)/sizeof(*array_size_list); i++) {
        RAM_output << format_size(array_size_list[i]) << ", ";
        for (int j=0; j < sizeof(stride_list)/sizeof(*stride_list); j++) {
            RAM_output << benchmark.get_latency(array_size_list[i], stride_list[j]) << ", ";
        }
        RAM_output << endl;
    }
    
    RAM_output.close();
    */
    
    /* 1. RAM Bandwidth */
    cout << "2. RAM Bandwidth" << endl;
    uint64_t bandwidth_size_list[] = {1 << 22, 1 << 23, 1 << 24, 1 << 25, 1 << 26, 1 << 27, 1 << 28, 1 << 29, 1 << 30};
    ofstream bandwidth_output;
    string bandwidth_name = "RAM_Bandwidth.csv";
    bandwidth_output.open(bandwidth_name);
    
    cout << "writing to " << bandwidth_name << endl;
    bandwidth_output << "Size, Read Bandwidth(GB/s), Write Bandwidth(GB/s)" << endl;
    
    for (int i = 0; i < sizeof(bandwidth_size_list)/sizeof(*bandwidth_size_list); i++) {
        bandwidth_output << format_size(bandwidth_size_list[i]) << ", ";
        bandwidth_output << benchmark.get_read_bandwidth(bandwidth_size_list[i]) << ", ";
        bandwidth_output << benchmark.get_write_bandwidth(bandwidth_size_list[i]) << endl;
    }
    
    return 0;
}