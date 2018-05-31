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

/**
 * DESC: Fork a bunch of asyncs in a top-level loop
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hclib.h"

#define NB_ASYNC 127

int ran[NB_ASYNC];

void async_fct(void * arg) {
    int idx = *((int *) arg);
    assert(ran[idx] == -1);
    ran[idx] = idx;
}

void entrypoint(void *arg) {
    int i = 0;
    // This is ok to have these on stack because this
    // code is alive until the end of the program.
    int indices [NB_ASYNC];
    for (i = 0; i < NB_ASYNC; i++) {
        ran[i] = -1;
    }

    hclib_start_finish();

    for (i = 0; i < NB_ASYNC; i++) {
        indices[i] = i;
        //Note: Forcefully pass the address we want to write to as a void **
        hclib_async(async_fct, (void*) (indices+i), NO_FUTURE, 0,
                ANY_PLACE);
    }

    hclib_end_finish();

    printf("Call Finalize\n");
}

int main (int argc, char ** argv) {
    printf("Call Init\n");
    char const *deps[] = { "system" };
    hclib_launch(entrypoint, NULL, deps, 1);
    printf("Check results: ");
    int i = 0;
    while(i < NB_ASYNC) {
        assert(ran[i] == i);
        i++;
    }
    printf("OK\n");
    return 0;
}
