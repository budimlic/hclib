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

#ifndef HCLIB_DEQUE_H_
#define HCLIB_DEQUE_H_

#include "hclib-task.h"
#include "hclib-atomics.h"

/****************************************************/
/* DEQUE API                                        */
/****************************************************/

#define STEAL_CHUNK_SIZE 1

<<<<<<< HEAD
// #define INIT_DEQUE_CAPACITY 16384
#define INIT_DEQUE_CAPACITY 262144

typedef struct hclib_internal_deque_t {
    /*
     * Head is shared by all threads, both stealers and the thread local to this
     * deque. Head points to the slot containing the oldest created task.
     * Stealing a task implies reading the task pointed to by head and then
     * safely incrementing head.
     */
    volatile int head;

    /*
     * Tail is only manipulated by the thread owning a deque. New tasks are
     * pushed into the slot pointed to by tail, followed by an increment of
     * tail. Local tasks may be acquired by decrementing tail and grabbing the
     * task at the slot pointed to post-decrement.
     */
    volatile int tail;

    volatile hclib_task_t* data[INIT_DEQUE_CAPACITY];
} hclib_internal_deque_t;

void deque_init(hclib_internal_deque_t *deq, void *initValue);
int deque_push(hclib_internal_deque_t *deq, void *entry);
hclib_task_t* deque_pop(hclib_internal_deque_t *deq);
int deque_steal(hclib_internal_deque_t *deq, void **stolen);
void deque_destroy(hclib_internal_deque_t *deq);
unsigned deque_size(hclib_internal_deque_t *deq);

#endif /* HCLIB_DEQUE_H_ */
