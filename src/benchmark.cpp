//
// Created by Yida Wang on 2/28/17.
//

#include "ctpl.h"
#include <stdio.h>

float* f1(size_t num_loops)
{
    float* arr = new float[num_loops];
    float* result = new float[num_loops];
#pragma omp parallel for
    for (size_t i=0; i<num_loops; i++){
        result[i] = arr[i] * 2.0;
    }
    return result;
}

int main(int argc, char** argv)
{
    float* result = f1(10000000000);
    printf("%.2f %.2f %.2f\n", result[0], result[10000], result[10000000]);
    return 0;
}