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

#include "hclib-internal.h"
#include "hclib-atomics.h"

void deque_init(hclib_internal_deque_t *deq, void *init_value) {
    deq->head = 0;
    deq->tail = 0;
}

/*
 * push an entry onto the tail of the deque
 */
int deque_push(hclib_internal_deque_t *deq, void *entry) {
    int size = deq->tail - deq->head;
    if (size == INIT_DEQUE_CAPACITY) { /* deque looks full */
        /* may not grow the deque if some interleaving steal occur */
        return 0;
    }
    const int n = (deq->tail) % INIT_DEQUE_CAPACITY;
    deq->data[n] = (hclib_task_t *) entry;

    // Required to guarantee ordering of setting data[n] with incrementing tail.
    hc_mfence();

    deq->tail++;
    return 1;
}

void deque_destroy(hclib_internal_deque_t *deq) {
    free(deq);
}

/*
 * The steal protocol. Returns the number of tasks stolen, up to
 * STEAL_CHUNK_SIZE. stolen must have enough space to store up to
 * STEAL_CHUNK_SIZE task pointers.
 */
int deque_steal(hclib_internal_deque_t *deq, void **stolen) {
    /* Cannot read deq->data[head] here
     * Can happen that head=tail=0, then the owner of the deq pushes
     * a new task when stealer is here in the code, resulting in head=0, tail=1
     * All other checks down-below will be valid, but the old value of the buffer head
     * would be returned by the steal rather than the new pushed value.
     */

    int nstolen = 0;

    int success;
    do {
        const int head = deq->head;
        hc_mfence();
        const int tail = deq->tail;

        if ((tail - head) <= 0) {
            success = 0;
        } else {
            hclib_task_t *t = (hclib_task_t *) deq->data[head % INIT_DEQUE_CAPACITY];
            /* compete with other thieves and possibly the owner (if the size == 1) */
            const int old = hc_cas(&deq->head, head, head + 1);
            if (old == head) {
                success = 1;
                stolen[nstolen++] = t;
            }

        }
    } while (success && nstolen < STEAL_CHUNK_SIZE);

    return nstolen;
}

/*
 * pop the task out of the deque from the tail
 */
hclib_task_t *deque_pop(hclib_internal_deque_t *deq) {
    hc_mfence();
    int tail = deq->tail;
    tail--;
    deq->tail = tail;
    hc_mfence();
    int head = deq->head;

    int size = tail - head;
    if (size < 0) {
        _hclib_atomic_store_relaxed(&deq->tail, head);
        return NULL;
    }
    hclib_task_t *t = (hclib_task_t *) deq->data[tail % INIT_DEQUE_CAPACITY];

    if (size > 0) {
        return t;
    }

    /* now size == 1, I need to compete with the thieves */
    const int old = hc_cas(&deq->head, head, head + 1);
    if (old != head) {
        t = NULL;
    }

    /* now the deque is empty */
    deq->tail = deq->head;
    return t;
}

unsigned deque_size(hclib_internal_deque_t *deq) {
    const int size = deq->tail - deq->head;
    if (size <= 0) return 0;
    else return (unsigned)size;
}
