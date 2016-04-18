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

    /*1 Measurement overhead*/
    cout << "1 Measurement overhead" << endl;
    cout << "1.1 Overhead of reading: " << benchmark.get_read_overhead() << " secs" << endl;
    cout << "1.2 Overhead of loop: " << benchmark.get_loop_overhead() << " secs" << endl;
    cout << endl;
    
    /*2. Procedure call overhead*/
    cout << "2. Procedure call overhead" << endl;
    for (int i = 0; i < 8; i++) {
        cout << "  Procudure call with " << i << " arguments: " << benchmark.get_procedure_call_overhead(i) << "secs" << endl;
    }
    cout << endl;


    return 0;
}

