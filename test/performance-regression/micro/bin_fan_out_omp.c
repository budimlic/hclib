#include "hclib.h"

#include <omp.h>
#include <stdio.h>
#include "bin_fan_out.h"

void recurse(const int depth) {
    if (depth < BIN_FAN_OUT_DEPTH) {
#pragma omp task default(none)
        {
            recurse(depth + 1);
        }

#pragma omp task default(none)
        {
            recurse(depth + 1);
        }
    }
}

int main(int argc, char **argv) {
    int i;

    int nthreads;
#pragma omp parallel default(none) shared(nthreads)
#pragma omp master
    {
        nthreads = omp_get_num_threads();
    }
    printf("Using %d OpenMP threads\n", nthreads);

#pragma omp parallel default(none)
#pragma omp master
    {
        const unsigned long long start_time = hclib_current_time_ns();

#pragma omp taskgroup
        recurse(0);

        const unsigned long long end_time = hclib_current_time_ns();
        printf("Did binary fan out of depth %d in OpenMP in %llu ns\n",
                BIN_FAN_OUT_DEPTH, end_time - start_time);
    }
}