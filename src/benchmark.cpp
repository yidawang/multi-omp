//
// Created by Yida Wang on 2/28/17.
//

#include "ctpl.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <glog/logging.h>
#include <omp.h>
#include <sys/time.h>

void f1(int tid, size_t num_loops, int iter) {
    int threads;
#pragma omp parallel
    {
    threads = omp_get_num_threads();
    }
    printf("%d\n", threads);
    kmp_affinity_mask_t mask;
    float* arr = new float[num_loops];
    float* result = new float[num_loops];
    size_t count = 0;
    float compiler_treat;
    kmp_create_affinity_mask(&mask);
    int max_proc = kmp_get_affinity_max_proc();
    for(int i=tid*threads;i<(tid+1)*threads; ++i) {
        CHECK_LT(max_proc-1-i, max_proc);
        CHECK_EQ(kmp_set_affinity_mask_proc(max_proc-1-i, &mask), 0);
    }
/*# pragma omp parallel num_threads(threads)
    {
        // set affinity for the spawned threads
        CHECK_EQ(kmp_set_affinity(&mask), 0);
    }*/
    while (count++ < iter) {
#pragma omp parallel num_threads(threads)
        {
            CHECK_EQ(kmp_set_affinity(&mask), 0);
#pragma omp for
            for (size_t i = 0; i < num_loops; i++) {
                result[i] = arr[i] * float(2.0);
            }
            compiler_treat = result[iter % num_loops];
            LOG(INFO) << sformat("in thread %d compiler_treat %.2f", tid, compiler_treat);
        }
    }
    delete [] arr;
    delete [] result;
    return;
}

void fill0(int tid, size_t arr_size, int iter) {
    int threads;
#pragma omp parallel
    {
        threads = omp_get_num_threads();
    }
    kmp_affinity_mask_t mask;
    float* result = new float[arr_size];
    size_t count = 0;
    float compiler_treat;
    kmp_create_affinity_mask(&mask);
    int max_proc = kmp_get_affinity_max_proc();
    for(int i=tid*threads;i<(tid+1)*threads; ++i) {
        CHECK_LT(i, max_proc);
        CHECK_EQ(kmp_set_affinity_mask_proc(i, &mask), 0);
    }
    struct timeval start, end;
    gettimeofday(&start, 0);
    while (count++ < iter) {
#pragma omp parallel num_threads(threads)
        {
            CHECK_EQ(kmp_set_affinity(&mask), 0);
#pragma omp for
#pragma vector nontemporal
            for (size_t i = 0; i < arr_size; i++) {
                result[i] = 0.0;
            }
        }
    }
    gettimeofday(&end, 0);
    double time_length = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;
    printf("time: %f bw: %.2f\n", time_length, sizeof(float) * arr_size / time_length * 1e-9 * iter);
    delete [] result;
}

int main(int argc, char** argv)
{
    size_t num_loops = 1000000000;
    size_t iter = 100;
    int inter_threads = atoi(argv[1]);
    //int intra_threads = int(argv[2]);
    ctpl::thread_pool pool(inter_threads);
    size_t arr_size = (size_t)atoi(argv[2]);
    //for (int i=0; i<inter_threads; i++) pool.push(f1, num_loops*(i+1), iter);
    for (int i=0; i<inter_threads; i++) pool.push(fill0, arr_size, iter);

    //float* result = f1(num_loops, iter);
    //LOG(INFO) << sformat("some results %.2f %.2f %.2f", result[0], result[10000], result[10000000]);
    return 0;
}