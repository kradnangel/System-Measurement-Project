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
#define TEST_TIMES 10
int main(int argc, const char * argv[]) {
    void* buf = malloc(BLOCK_SIZE);
    unsigned long size[5] = {16777216, 33554432, 67108864, 134217728, 268435456}; // 4KB 8KB 16KB 32KB 64KB 128KB 256KB 512KB 1MB 2MB 4MB 8MB 16MB 32MB 64MB 128MB 256MB 

    //sequential access
    for (int i = 0; i < 5; i++) {
        uint64_t begin, end, sum = 0;   // used to calculate the time used 
        unsigned int block_numbers = size[i] / BLOCK_SIZE;
        for (int j = 0; j < TEST_TIMES; j++) {
            system("sudo purge");           // force disk cache to be purged (flushed and emptied) at first. 
            int file = open("/Users/zhouqianren/Documents/CSE_221_OS/Proj/LargeTestFile", O_SYNC | O_RDONLY);
            fcntl(file, F_NOCACHE, 1);      // Turns data caching off/on.

            for (int j = 0; j < block_numbers; j++) {
                begin = rdtsc();
                read(file, buf, BLOCK_SIZE);
                end = rdtsc();
                // if (j % 100 == 0){ printf("%llu\n", (end-begin));}
                sum += (end - begin);
            } 
            close(file);
        }
        printf("4.2 File sequential access time on %lu MB file: %.0llu \n", size[i]/1024/1024, sum / block_numbers / TEST_TIMES);
    }
    
    //random access
    for (int i = 0; i < 5; i++) {
        uint64_t begin, end, sum = 0;   // used to calculate the time used 
        unsigned int block_numbers = size[i] / BLOCK_SIZE;
        for (int j = 0; j < TEST_TIMES; j++) {
            system("sudo purge");           // force disk cache to be purged (flushed and emptied) at first. 
            int file = open("/Users/zhouqianren/Documents/CSE_221_OS/Proj/LargeTestFile", O_SYNC | O_RDONLY);
            fcntl(file, F_NOCACHE, 1);     // Turns data caching off/on.

            off_t offset;
            for (int j = 0; j < block_numbers; j++) {
                offset = (off_t) (rand() % block_numbers * BLOCK_SIZE);
                // printf("%lld\n",offset);
                begin = rdtsc();
                //off_t lseek(int fd, off_t offset, int whence);
                lseek(file, offset, SEEK_SET);    // SEEK_SET -- The file offset is set to offset bytes.
                read(file, buf, BLOCK_SIZE);
                end = rdtsc();
                // if (j % 100 == 0){ printf("%llu\n", (end-begin));}
                sum += (end - begin);
            } 
            close(file);
        }
        printf("4.2 File random access time on %lu MB file: %.0llu \n", size[i]/1024/1024, sum / block_numbers / TEST_TIMES);
    }
    

    
    return 0; 
}