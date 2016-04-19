//
//  benchmark.h
//  CPU_measurement
//
//  Created by Kun on 4/17/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#ifndef __CPU_measurement__benchmark__
#define __CPU_measurement__benchmark__

#define SAMPLE_SIZE 10000
#define SAMPLE_RATIO 0.2

#define TASK_SAMPLE_SIZE 10

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <pthread.h>

class CPUBenchmark {
    
private:
    double read_overhead;
    double loop_overhead;
    uint64_t cycles[SAMPLE_SIZE];
    double get_sampled_average(uint64_t cycles[], int size=SAMPLE_SIZE, double ratio=SAMPLE_RATIO);

public:
    CPUBenchmark();
    double get_read_overhead();
    double get_loop_overhead(int times=SAMPLE_SIZE);
    double get_procedure_call_overhead(int num_arguments);
    double get_system_call_overhead_0();
    double get_system_call_overhead_1();
    double get_measurement_overhead();
    double get_process_creation_overhead();
    double get_kernel_thread_creation_overhead();
    double get_process_context_switch_time();
    double get_thread_context_switch_time();
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

#endif /* defined(__CPU_measurement__benchmark__) */
