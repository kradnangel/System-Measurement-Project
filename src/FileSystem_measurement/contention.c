#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>      //manipulate file descriptor
#include <string.h>

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
#define SIZE 268435456 // 256MB 
#define BLOCK_NUM (SIZE / BLOCK_SIZE)
uint64_t fileRead(char index) {

    void* buf = malloc(BLOCK_SIZE);
    uint64_t begin, end, sum = 0;   // used to calculate the time used 
    char str[80];
    strcpy(str, "/Users/zhouqianren/Documents/CSE_221_OS/Proj/TestFile");
    str[53] = index;
    
    for (int j = 0; j < TEST_TIMES; j++) {
        int file = open(str, O_SYNC | O_RDONLY);
        fcntl(file, F_NOCACHE, 1);
        for (int j = 0; j < BLOCK_NUM; j++) {
            begin = rdtsc();
            read(file, buf, BLOCK_SIZE);
            end = rdtsc();
            // if (j % 100 == 0){ printf("%llu\n", (end-begin));}
            sum += (end - begin);
        } 
        close(file);            
    }
    printf("%c %llu\n", index, sum);
    return sum;
}

int main(int argc, const char * argv[]) {
    uint64_t sum = 0;   // used to calculate the time used 
    int FILE_NUM = atoi(argv[1]);
    pid_t pid[FILE_NUM];
    for (int i = 1; i < FILE_NUM; i++) {
        /* fork a child process */
        pid[i-1] = fork();
        if (pid[i-1] < 0) { /* error occurred */ 
            fprintf(stderr, "Fork Failed"); return 1;
        }else if (pid[i-1] == 0) { /* child process */ 
            // printf("%d\n", i);
            fileRead('0'+i);
            printf("Process %d is done.\n", i);
            exit(0);
        }
    }
    /* parent process */
    sum = fileRead('0'+0);
    printf("4.4 file read time on 256 MB file: %.0llu when %d process(es) running\n", sum / BLOCK_NUM / TEST_TIMES, FILE_NUM);
    
    return 0; 
}