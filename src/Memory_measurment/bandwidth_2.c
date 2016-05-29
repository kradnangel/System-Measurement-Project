#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>

#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>
#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

#define SIZE 2.0
#define GB 1073741824

int main() {
    uint64_t        begin, end, sum;
    uint64_t        elapsed;
    Nanoseconds     elapsedNano;

    char *a = (char *)malloc(SIZE * GB * sizeof(char));
    begin = mach_absolute_time();
    memset(a, 'a', SIZE*GB);
    end = mach_absolute_time();
    elapsed = end - begin;
    elapsedNano = AbsoluteToNanoseconds( *(AbsoluteTime *) &elapsed );
    sum = * (uint64_t *) &elapsedNano;
    
    printf("Bandwidth: %.2lf (array size = %d GB)\n", 1.0*SIZE/sum, SIZE);

    free(a);
    return 0;
}