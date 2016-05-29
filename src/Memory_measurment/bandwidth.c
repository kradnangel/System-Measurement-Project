#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <mach/mach_time.h>
#include <inttypes.h>

#define SIZE 1
#define GB (long)1073741824

#define NANOS_PER_SECF 1000000000.0
static mach_timebase_info_data_t info;
static void __attribute__((constructor)) init_info() {
  mach_timebase_info(&info);
}
double monotonic_time() {
    uint64_t time = mach_absolute_time();
    double dtime = (double) time;
    dtime *= (double) info.numer;
    dtime /= (double) info.denom;
    return dtime / NANOS_PER_SECF;
}

int main() {
    double begin, end;
    char *a = (char *)malloc(SIZE * GB * sizeof(char));
    char c;

    memset(a, 0xFF, SIZE * GB); //flush
    begin = monotonic_time();
    // memset(a, 0x01, SIZE * GB);
    for (long i = 0; i < SIZE * GB; i+= 64) {
        a[i] = 0x00;    
    }
    end = monotonic_time();
    printf("Memory Bandwidth for writing: %.2lf (array size = %d GB)\n", 1.0*SIZE/(end-begin), SIZE);

    memset(a, 0xFF, SIZE * GB); //flush
    begin = monotonic_time();
    for (long i = 0; i < SIZE * GB; i+= 64) {
        c = a[i];
    }
    end = monotonic_time();    
    printf("Memory Bandwidth for reading: %.2lf (array size = %d GB)\n", 1.0*SIZE/(end-begin), SIZE);

    free(a);
    return 0;
}