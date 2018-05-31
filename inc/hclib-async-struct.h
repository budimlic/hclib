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

#ifndef HCLIB_ASYNCSTRUCT_H_
#define HCLIB_ASYNCSTRUCT_H_

#include <string.h>

#include "hclib-task.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void spawn(hclib_task_t * task);
extern void spawn_await_at(hclib_task_t *task, hclib_future_t **futures,
        const int nfutures, hclib_locale_t *locale);
extern void spawn_at(hclib_task_t *task, hclib_locale_t *locale);
extern void spawn_await(hclib_task_t *task, hclib_future_t **futures,
        const int nfutures);

#ifdef __cplusplus
}
#endif

#endif /* HCLIB_ASYNCSTRUCT_H_ */
