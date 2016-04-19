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

    /* 1. Measurement overhead */
    cout << "1. Measurement overhead" << endl;
    cout << "1.1. Overhead of reading: " << benchmark.get_read_overhead() << " cycles" << endl;
    cout << "1.2. Overhead of loop: " << benchmark.get_loop_overhead() << " cycles" << endl;
    cout << endl;
    
    /* 2. Procedure call overhead */
    cout << "2. Procedure call overhead" << endl;
    for (int i = 7; i >= 0; i--) {
        cout << "  Procedure call with " << i << " arguments: " << benchmark.get_procedure_call_overhead(i) << " cycles" << endl;
    }
    cout << endl;

    /* 3. System call overhead */
    cout << "3. System call overhead" << endl;
    cout << "  Cost of getpid() system call: " << benchmark.get_system_call_overhead_0() << " cycles" << endl;
    cout << "  Cost of getppid() system call: " << benchmark.get_system_call_overhead_1() << " cycles" << endl;
    cout << endl;

    /* 4. Task creation time */
    cout << "4. Task creation time" << endl;
    cout << "  Process creation cost: " << endl;
    cout << benchmark.get_process_creation_overhead() << " cycles" << endl;
    cout << "  Thread creation cost: " << endl;
    cout << benchmark.get_kernel_thread_creation_overhead() << " cycles" << endl;
    cout << endl;
    
    /* 5. Context switch time */
    cout << "5. Context switch time" << endl;
    cout << "5.1. Process context switch time: " << endl;
    cout << benchmark.get_process_context_switch_time() << " cycles" << endl;
    cout << "5.2. Thread context switch time: " << benchmark.get_thread_context_switch_time() << " cycles" << endl;
    cout << endl;
    
    return 0;
}

