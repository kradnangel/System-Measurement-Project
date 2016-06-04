//
//  helper.h
//  Network_measurement
//
//  Created by Kun on 6/3/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#ifndef __Network_measurement__helper__
#define __Network_measurement__helper__

#include <iostream>
#include <math.h>

float standard_deviation(double data[], int n)
{
    double mean=0.0, sum_deviation=0.0;
    int i;
    for(i=0; i<n;++i)
    {
        mean+=data[i];
    }
    mean=mean/n;
    for(i=0; i<n;++i)
        sum_deviation+=(data[i]-mean)*(data[i]-mean);
    return sqrt(sum_deviation/n);
}

float average(double data[], int n)
{
    double mean=0.0;
    int i;
    for(i=0; i<n;++i)
    {
        mean+=data[i];
    }
    mean=mean/n;
    return mean;
}

float min(double data[], int n)
{
    double min=data[0];
    int i;
    for(i=0; i<n;++i)
    {
        if (data[i] < min)
            min = data[i];
    }
    return min;
}

float max(double data[], int n)
{
    double max=data[0];
    int i;
    for(i=0; i<n;++i)
    {
        if (data[i] > max)
            max = data[i];
    }
    return max;
}

#endif /* defined(__Network_measurement__helper__) */
