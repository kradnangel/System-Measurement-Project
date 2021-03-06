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
    
    unsigned int FILE_SIZE = 268435456; // 256 MB
    char c;
    uint64_t begin, end, sum = 0;
    
    sum = 0;
    for (unsigned int h = 0; h < SAMPLE_SIZE; h++) {
        system("sudo purge");    
        int file = open("/Users/zhouqianren/Documents/CSE_221_OS/Proj/TestFile", O_RDWR);
        char* target = (char*) mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
        
        begin = rdtsc();
        c = target[0]; 
        end = rdtsc();
        sum += (end - begin);

        munmap(target, FILE_SIZE);
        close(file);
    }

    printf("2.3 Page fault service time: %.0lf \n", round(sum/SAMPLE_SIZE));

    return 0;
}