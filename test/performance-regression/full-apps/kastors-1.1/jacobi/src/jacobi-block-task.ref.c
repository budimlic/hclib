#include <sys/time.h>
#include <time.h>
#include <stdio.h>
static unsigned long long current_time_ns() {
#ifdef __MACH__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    unsigned long long s = 1000000000ULL * (unsigned long long)mts.tv_sec;
    return (unsigned long long)mts.tv_nsec + s;
#else
    struct timespec t ={0,0};
    clock_gettime(CLOCK_MONOTONIC, &t);
    unsigned long long s = 1000000000ULL * (unsigned long long)t.tv_sec;
    return (((unsigned long long)t.tv_nsec)) + s;
#endif
}
# include "poisson.h"

/* #pragma omp task/taskwait version of SWEEP. */
void sweep (int nx, int ny, double dx, double dy, double *f_,
            int itold, int itnew, double *u_, double *unew_, int block_size)
{
    int it;
    int block_x, block_y;

    if (block_size == 0)
        block_size = nx;

    int max_blocks_x = (nx / block_size);
    int max_blocks_y = (ny / block_size);

#pragma omp parallel shared(u_, unew_, f_, max_blocks_x, max_blocks_y, nx, ny, dx, dy, itold, itnew, block_size) private(it, block_x, block_y)
{
#pragma omp single 
{
        for (it = itold + 1; it <= itnew; it++) {
            // Save the current estimate.
            for (block_x = 0; block_x < max_blocks_x; block_x++) {
                for (block_y = 0; block_y < max_blocks_y; block_y++) {
#pragma omp task shared(u_, unew_, nx, ny, block_size) firstprivate(block_x, block_y)
copy_block(nx, ny, block_x, block_y, u_, unew_, block_size);
                }
            }

#pragma omp taskwait 
;

            // Compute a new estimate.
            for (block_x = 0; block_x < max_blocks_x; block_x++) {
                for (block_y = 0; block_y < max_blocks_y; block_y++) {
#pragma omp task default(none) shared(u_, unew_, f_, dx, dy, nx, ny, block_size) firstprivate(block_x, block_y)
compute_estimate(block_x, block_y, u_, unew_, f_, dx, dy,
                                     nx, ny, block_size);
                }
            }

#pragma omp taskwait 
;
        }
    }
    }
}