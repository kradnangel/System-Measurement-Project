#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>      //manipulate file descriptor

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

#define BLOCK_SIZE 4096

int main(int argc, const char * argv[]) {
    // unsigned long size[6] = {268435456, 536870912, 1073741824, 2147483648, 4294967296, 8589934592}; // 256MB 512MB 1G 2G 4G 8G
    unsigned long size[5] = {2147483648, 3221225472, 4294967296, 5368709120, 6442450944}; //2G 3G 4G 5G 6G 
    void* buf = malloc(BLOCK_SIZE);

    for (int i = 0; i < 5; i++) {
        //preprocess
        uint64_t begin, end, sum = 0;   // used to calculate the time used 
        system("sudo purge");           // force disk cache to be purged (flushed and emptied) at first. 
        int file = open("/Users/zhouqianren/Documents/CSE_221_OS/Proj/LargeTestFile", O_RDWR);
        
        //Read file from disk to file cache 
        unsigned int read_times = size[i] / BLOCK_SIZE;
        printf("%lu %d\n", size[i], read_times);
        for (int j = 0; j < read_times; j++) {
            read(file, buf, BLOCK_SIZE); //read from a file descriptor
        }

        //test
        close(file);
        file = open("/Users/zhouqianren/Documents/CSE_221_OS/Proj/LargeTestFile", O_RDWR);
        for (int j = 0; j < read_times; j++) {
            begin = rdtsc();
            read(file, buf, BLOCK_SIZE);
            end = rdtsc();
            // if (j % 100 == 0){ printf("%llu\n", (end-begin));}
            sum += (end - begin);
        }        
        printf("4.1 Time spent on %lu MB file: %.0llu \n", size[i]/1024/1024, sum / read_times);

        close(file);
    }
    
    return 0; 
}