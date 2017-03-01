//
// Created by Yida Wang on 2/28/17.
//

#include "ctpl.h"
#include "util.h"
#include <stdio.h>
#include <glog/logging.h>

void f1(int tid, size_t num_loops, int iter)
{
    float* arr = new float[num_loops];
    float* result = new float[num_loops];
    int count = 0;
    float compiler_treat;
    while (count++ < iter) {
#pragma omp parallel for
        for (size_t i = 0; i < num_loops; i++) {
            result[i] = arr[i] * 2.0;
        }
        compiler_treat = result[iter];
        LOG(INFO) << sformat("in thread %d compiler_treat %.2f", tid, compiler_treat);
    }
    delete [] arr;
    delete [] result;
    return;
}

int main(int argc, char** argv)
{
    size_t num_loops = 1000000000;
    int iter = 100;
    int inter_threads = 4;
    //int intra_threads = int(argv[2]);
    ctpl::thread_pool pool(inter_threads);
    for (int i=0; i<inter_threads; i++) pool.push(f1, num_loops, iter);

    //float* result = f1(num_loops, iter);
    //LOG(INFO) << sformat("some results %.2f %.2f %.2f", result[0], result[10000], result[10000000]);
    return 0;
}