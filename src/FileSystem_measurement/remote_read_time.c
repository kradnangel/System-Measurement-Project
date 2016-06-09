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
#define TEST_TIMES 1
int main(int argc, const char * argv[]) {
    void* buf = malloc(BLOCK_SIZE);
    unsigned long size[5] = {16777216, 33554432, 67108864, 134217728, 268435456}; // 16MB 32MB 64MB 128MB 256MB 
    uint64_t begin, end, sum = 0;   // used to calculate the time used 
    unsigned int block_numbers;
    
    for (int i = 0; i < 5; i++) {
        block_numbers = size[i] / BLOCK_SIZE;
        //sequential access
        sum = 0;    
        for (int j = 0; j < TEST_TIMES; j++) {
            system("sudo purge");           // force disk cache to be purged (flushed and emptied) at first. 
            int file = open("/Users/zhouqianren/FromSharing/LargeTestFile", O_SYNC | O_RDONLY);
            if(fcntl(file, F_NOCACHE, 1) < 0) {
                printf("%s\n",  "Can't close cache of the test file");
                break;
            }
            for (int j = 0; j < block_numbers; j++) {
                begin = rdtsc();
                read(file, buf, BLOCK_SIZE);
                end = rdtsc();
                // if (j % 100 == 0){ printf("%llu\n", (end-begin));}
                sum += (end - begin);
            } 
            close(file);
        }
        printf("4.3 Remote file sequential access time on %lu MB file: %.0llu \n", size[i]/1024/1024, sum / block_numbers / TEST_TIMES);
    
        //random access
        sum = 0;
        for (int j = 0; j < TEST_TIMES; j++) {
            system("sudo purge");           // force disk cache to be purged (flushed and emptied) at first. 
            int file = open("/Users/zhouqianren/FromSharing/LargeTestFile", O_SYNC | O_RDONLY);
            if(fcntl(file, F_NOCACHE, 1) < 0) {
                printf("%s\n",  "Can't close cache of the test file");
                break;
            }
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
        printf("4.3 Remote file random access time on %lu MB file: %.0llu \n", size[i]/1024/1024, sum / block_numbers / TEST_TIMES);
    }
    
    return 0; 
}