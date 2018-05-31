/*
 * Copyright 2017 Rice University
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "litectx.h"

#ifndef HCLIB_RT_H_
#define HCLIB_RT_H_

#ifdef __cplusplus
extern "C" {
#endif

// forward declaration
extern pthread_key_t ws_key;
struct hc_context;
struct hclib_options;
struct place_t;
struct hclib_deque_t;
struct finish_t;
struct _hclib_worker_paths;

#ifdef USE_HWLOC
// Various thread affinities supported by HClib
typedef enum {
    /*
     * In HCLIB_AFFINITY_STRIDED mode the HClib runtime will take the set of
     * CPUs it has been given by the calling driver (e.g. taskset, srun, aprun)
     * and set thread affinities such that each thread has the Nth bit,
     * (N+nthreads)th bit, (N+2*nthreads)th bit, etc. set. It is an error to set
     * this affinity with fewer CPUs than there are runtime threads.
     */
    HCLIB_AFFINITY_STRIDED,
    /*
     * In HCLIB_AFFINITY_CHUNKED mode, the HClib runtime will chunk together the
     * set bits in a thread's CPU mask.
     */
    HCLIB_AFFINITY_CHUNKED
} hclib_affinity_t;
#endif

typedef struct _hclib_worker_state {
    // Global context for this instance of the runtime.
    struct hclib_context *context;
    // Pop and steal paths for this worker to traverse when looking for work.
    struct _hclib_worker_paths *paths;
    // The pthread associated with this worker context.
    pthread_t t;
    // Finish scope for the currently executing task.
    struct finish_t* current_finish;
    // Stack frame we are currently executing on.
    LiteCtx *curr_ctx;
    // Root context of the whole runtime instance.
    LiteCtx *root_ctx;
    // The id, identify a worker.
    int id;
    // Total number of workers in this instance of the HClib runtime.
    int nworkers;
    // Place to keep module-specific per-worker state.
    char *module_state;
    /*
     * Variables holding worker IDs for a contiguous chunk of workers that share
     * a NUMA node with this worker, and who we should therefore try to steal
     * from first.
     */
    int base_intra_socket_workers;
    int limit_intra_socket_workers;

    /*
     * Information on currently executing task.
     */
    void *curr_task;
} __attribute__ ((aligned (128))) hclib_worker_state;

#define HCLIB_MACRO_CONCAT(x, y) _HCLIB_MACRO_CONCAT_IMPL(x, y)
#define _HCLIB_MACRO_CONCAT_IMPL(x, y) x ## y

#ifdef HC_ASSERTION_CHECK
#define HASSERT(cond) { \
    if (!(cond)) { \
        if (pthread_getspecific(ws_key)) { \
            fprintf(stderr, "W%d: assertion failure\n", hclib_get_current_worker()); \
        } \
        assert(cond); \
    } \
}
#else
#define HC_ASSERTION_CHECK_ENABLED 0
#endif

#define HASSERT(cond) do { \
    if (HC_ASSERTION_CHECK_ENABLED) { \
        if (!(cond)) { \
            fprintf(stderr, "W%d: assertion failure\n", get_current_worker()); \
            assert(0 && (cond)); \
        } \
    } \
} while (0)

#if __cplusplus // C++11 static assert
#define HASSERT_STATIC static_assert
#else // C11 static assert
#define HASSERT_STATIC _Static_assert
#endif

#define CURRENT_WS_INTERNAL ((hclib_worker_state *) pthread_getspecific(ws_key))

int hclib_get_current_worker();
hclib_worker_state* current_ws();

typedef void (*generic_frame_ptr)(void*);

#include "hclib-timer.h"
#include "hclib-promise.h"

int  hclib_get_num_workers();
void hclib_start_finish();
void hclib_end_finish();
void hclib_user_harness_timer(double dur);

#ifdef __cplusplus
}
#endif

#endif
