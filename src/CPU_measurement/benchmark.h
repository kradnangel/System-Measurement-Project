//
//  benchmark.h
//  CPU_measurement
//
//  Created by Kun on 4/17/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#ifndef __CPU_measurement__benchmark__
#define __CPU_measurement__benchmark__

#define TIMES 100000

#include <iostream>

class CPUBenchmark {
    
private:
    double read_overhead;
    double loop_overhead;

public:
    CPUBenchmark();
    double get_read_overhead();
    double get_loop_overhead(int times=TIMES);
    double get_procedure_call_overhead(int num_arguments);
    double get_measurement_overhead();
};


// template procudure calls with various number of argurment
static void fun_0(){}
static void fun_1(int arg1){}
static void fun_2(int arg1, int arg2){}
static void fun_3(int arg1, int arg2, int arg3){}
static void fun_4(int arg1, int arg2, int arg3, int arg4){}
static void fun_5(int arg1, int arg2, int arg3, int arg4, int arg5){}
static void fun_6(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6){}
static void fun_7(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7){}

#endif /* defined(__CPU_measurement__benchmark__) */
