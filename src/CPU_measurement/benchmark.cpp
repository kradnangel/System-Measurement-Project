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
    uint64_t begin, end;
    pid_t pid;

    for(int i = 0; i < TASK_SAMPLE_SIZE; i++) {
        // stop counting
        begin = rdtsc();
        pid = fork();
        if(pid == 0) {
            // exit as it's child process
            exit(1);
        }
        else {
            // stop counting
            end = rdtsc();
            sum += end-begin;
            // wait child exit
            wait(NULL);
        }
    }
    
    return sum/TASK_SAMPLE_SIZE-read_overhead;
}

double CPUBenchmark::get_kernel_thread_creation_overhead() {
    uint64_t begin, end;
    pthread_t thread;
    void* thread_rountine(void *ptr);
    
    begin = rdtsc();
    for(int i = 0; i < TASK_SAMPLE_SIZE; i++) {
        pthread_create(&thread, NULL, &thread_rountine, NULL);
        // wait for threads
        pthread_join(thread, NULL);
    }
    end = rdtsc();
    
    return (end-begin-read_overhead)/TASK_SAMPLE_SIZE-loop_overhead;
}

double CPUBenchmark::get_process_context_switch_time_once(){
    int fd[2];
    pipe(fd);
    uint64_t begin;
    uint64_t end;
    pid_t pid;

    if ((pid = fork()) != 0) {
        begin = rdtsc();
        wait(NULL);
        read(fd[0], (void*)&end, sizeof(uint64_t));
    }
    else {
        end = rdtsc();
        write(fd[1], (void*)&end, sizeof(uint64_t));
        exit(1);
    }
    if(end > begin){
        return end - begin;
    }
    return 0;
}

double CPUBenchmark::get_process_context_switch_time(){
    double sum;
    double cycles;
    
    for (int i=0; i<TASK_SAMPLE_SIZE; i++) {
        while((cycles=get_process_context_switch_time_once())<=0){};
        sum += cycles;
    }
    
    return sum/TASK_SAMPLE_SIZE;
}

double CPUBenchmark::get_thread_context_switch_time_once(){
    uint64_t begin;
    uint64_t end;
    pthread_t thread;
    void* thread_set_end(void * result);

    pthread_create(&thread, NULL, thread_set_end, &end);
    begin = rdtsc();
    pthread_join(thread, NULL);
    
    if (end > begin)
        return end - begin;
    return 0;
}

double CPUBenchmark::get_thread_context_switch_time(){
    double sum;
    double cycles;
    
    for (int i=0; i<TASK_SAMPLE_SIZE; i++) {
        while((cycles=get_thread_context_switch_time_once())<=0){};
        sum += cycles;
    }
    
    return sum/TASK_SAMPLE_SIZE;
}

void* thread_set_end(void * result) {
    uint64_t end = rdtsc();
    *((uint64_t*)result) = end;
    pthread_exit(NULL);
}

void* thread_rountine(void *ptr) {
    // avoid of overhead
    pthread_exit(0);
}