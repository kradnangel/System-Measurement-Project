//
//  benchmark.h
//  CPU_measurement
//
//  Created by Kun on 4/17/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#ifndef __CPU_measurement__benchmark__
#define __CPU_measurement__benchmark__

#include <iostream>

class CPUBenchmark {
    
private:

public:
    double get_read_overhead();
    double get_loop_overhead(int times);
    double get_measurement_overhead();
};

#endif /* defined(__CPU_measurement__benchmark__) */
