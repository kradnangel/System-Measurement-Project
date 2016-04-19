//
//  benchmark.cpp
//  CPU_measurement
//
//  Created by Kun on 4/17/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include "benchmark.h"

using namespace std;

CPUBenchmark::CPUBenchmark() {
    read_overhead = get_read_overhead();
    loop_overhead = get_loop_overhead(SAMPLE_SIZE);
}

double CPUBenchmark::get_sampled_average(uint64_t *cycles, int size, double ratio)
{
    double sum;
    sort(cycles, cycles+size);
    for (int i=0; i < size*ratio; i++) {
        sum += cycles[i];
    }
    return sum/(size*ratio);
}

double CPUBenchmark::get_read_overhead() {
    uint64_t begin, end;
    for(int i = 0; i < SAMPLE_SIZE; i++) {
        begin = rdtsc();
        end = rdtsc();
        cycles[i] = end-begin;
    }
    return get_sampled_average(cycles);
}

double CPUBenchmark::get_loop_overhead(int times) {
    uint64_t begin, end;
    begin = rdtsc();
    for(int i = 0; i < times; i++) {}
    end = rdtsc();
    
    return ((end - begin) - read_overhead) / times;
}

double CPUBenchmark::get_procedure_call_overhead(int num_arguments){
    uint64_t begin, end;
    switch (num_arguments) {
        case 0:
            begin = rdtsc();
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                fun_0();
                fun_0();
                fun_0();
                fun_0();
                fun_0();
            }
            end = rdtsc();
            break;
        case 1:
            begin = rdtsc();
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                fun_1(i);
                fun_1(i);
                fun_1(i);
                fun_1(i);
                fun_1(i);
            }
            end = rdtsc();
            break;
        case 2:
            begin = rdtsc();
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                fun_2(i, i);
                fun_2(i, i);
                fun_2(i, i);
                fun_2(i, i);
                fun_2(i, i);
            }
            end = rdtsc();
            break;
        case 3:
            begin = rdtsc();
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                fun_3(i, i, i);
                fun_3(i, i, i);
                fun_3(i, i, i);
                fun_3(i, i, i);
                fun_3(i, i, i);
            }
            end = rdtsc();
            break;
        case 4:
            begin = rdtsc();
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                fun_4(i, i, i, i);
                fun_4(i, i, i, i);
                fun_4(i, i, i, i);
                fun_4(i, i, i, i);
                fun_4(i, i, i, i);
            }
            end = rdtsc();
            break;
        case 5:
            begin = rdtsc();
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                fun_5(i, i, i, i, i);
                fun_5(i, i, i, i, i);
                fun_5(i, i, i, i, i);
                fun_5(i, i, i, i, i);
                fun_5(i, i, i, i, i);
            }
            end = rdtsc();
            break;
        case 6:
            begin = rdtsc();
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                fun_6(i, i, i, i, i, i);
                fun_6(i, i, i, i, i, i);
                fun_6(i, i, i, i, i, i);
                fun_6(i, i, i, i, i, i);
                fun_6(i, i, i, i, i, i);
            }
            end = rdtsc();
            break;
        case 7:
            begin = rdtsc();
            for (int i = 0; i < SAMPLE_SIZE; i++)
            {
                fun_7(i, i, i, i, i, i, i);
                fun_7(i, i, i, i, i, i, i);
                fun_7(i, i, i, i, i, i, i);
                fun_7(i, i, i, i, i, i, i);
                fun_7(i, i, i, i, i, i, i);
            }
            end = rdtsc();
            break;
        default:
            // unanticipated input
            return -1;
            break;
    }
    return ((end-begin-read_overhead)/SAMPLE_SIZE-loop_overhead)/5;
    return get_sampled_average(cycles) - read_overhead;
}

double CPUBenchmark::get_system_call_overhead_0(){
    /*
     http://man7.org/linux/man-pages/man2/getpid.2.html
     getpid() returns the process ID of the calling process.  (This is
     often used by routines that generate unique temporary filenames.)
     */
    uint64_t begin, end;

    begin = rdtsc();
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        getpid();
        getpid();
        getpid();
        getpid();
        getpid();
    }
    end = rdtsc();
    return ((end-begin-read_overhead)/SAMPLE_SIZE-loop_overhead)/5;
}

double CPUBenchmark::get_system_call_overhead_1(){
    /*
     http://man7.org/linux/man-pages/man2/getpid.2.html
     getpid() returns the process ID of the calling process.  (This is
     often used by routines that generate unique temporary filenames.)
     */
    uint64_t begin, end;
    
    begin = rdtsc();
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        getppid();
        getppid();
        getppid();
        getppid();
        getppid();
    }
    end = rdtsc();
    return ((end-begin-read_overhead)/SAMPLE_SIZE-loop_overhead)/5;
}

double CPUBenchmark::get_process_creation_overhead() {
    uint64_t sum;
    uint64_t start, end;
    pid_t pid;

    for(int i = 0; i < TASK_SAMPLE_SIZE; i++) {
        start = rdtsc();
        pid = fork();
        if(pid == 0) {
            // child process, just exit
            exit(1);
        }
        else {
            end = rdtsc();
            sum += end-start;
            // parent process, wait child exit
            wait(NULL);
        }
    }
    
    return sum/TASK_SAMPLE_SIZE-read_overhead;
}

double CPUBenchmark::get_process_context_switch_time(){
    int fd[2];
    pipe(fd);
    uint64_t start;
    uint64_t end;
    pid_t cpid;
    uint64_t result = 0;

    if ((cpid = fork()) != 0) {
        start = rdtsc();
        wait(NULL);
        read(fd[0], (void*)&end, sizeof(uint64_t));
    }
    else {
        end = rdtsc();
        write(fd[1], (void*)&end, sizeof(uint64_t));
        exit(1);
    }
    if(end > start){
        result = end - start;
    }
    return result;
}

double CPUBenchmark::get_thread_context_switch_time(){
    uint64_t start;
    uint64_t end;
    
    pthread_t thread;
    
    void* foo(void * result);

    pthread_create(&thread, NULL, foo, &end);
    
    start = rdtsc();
    pthread_join(thread, NULL);
    
    return end - start;
}

void* foo(void * result) {
    uint64_t end = rdtsc();
    *((uint64_t*)result) = end;
    pthread_exit(NULL);
}
