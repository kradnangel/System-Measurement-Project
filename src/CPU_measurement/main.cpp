//
//  main.cpp
//  CPU_measurement
//
//  Created by Kun on 4/17/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include <iostream>
#include "benchmark.h"

using namespace std;

int main(int argc, const char * argv[])
{
    CPUBenchmark benchmark;
    
    cout << "1 Measurement overhead" << endl;
    cout << "1.1 Overhead of reading: " << benchmark.get_read_overhead() << " secs" << endl;
    cout << "1.2 Overhead of loop (1,000,000 loops): " << benchmark.get_loop_overhead(1000000)/1000000 << " secs" << endl;
    

    return 0;
}

