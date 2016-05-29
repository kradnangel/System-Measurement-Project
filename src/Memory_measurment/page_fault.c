#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>

#define SAMPLE_SIZE 10.0

static inline uint64_t rdtsc(){
    uint64_t tsc;
    __asm__ __volatile__("rdtscp; "         // serializing read of tsc
                         "shl $32,%%rdx; "  // shift higher 32 bits stored in rdx up
                         "or %%rdx,%%rax"   // and or onto rax
                         : "=a"(tsc)        // output to tsc variable
                         :
                         : "%rcx", "%rdx"); // rcx and rdx are clobbered
    return tsc;
}

int main() {
    //unsigned int PAGE_SIZE = 4 * 1024;   // 4 KB
    
    unsigned int FILE_SIZE = 3758096384; // 3.5 GB
    char c;
    uint64_t begin, end, sum = 0;
    unsigned int stride[10] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 1024};
    
    for (unsigned int h = 0; h < 10; h++) {
        sum = 0;
        system("sudo purge");    
        int file = open("/Users/zhouqianren/Documents/CSE_221_OS/Proj/LargeTestFile", O_RDWR);
        char* target = (char*) mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
        // c = target[ (0) * stride[h] * 1024 ]; 
        for (int i = 0; i < SAMPLE_SIZE; i++) {   
            begin = rdtsc();
            c = target[ (i+1) * stride[h] * 1024 ]; 
            end = rdtsc();
            sum += (end - begin);
        }
        printf("2.3 Page fault service time: %.0lf (stride size = %d KB)\n", round(sum/SAMPLE_SIZE), stride[h]);
        munmap(target, FILE_SIZE);
        close(file);
    }

    return 0;
}