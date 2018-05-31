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
 * DESC: Top-level async spawn
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hclib.hpp"

using namespace hclib;

int ran = 0;

int main (int argc, char ** argv) {
    const char *deps[] = { "system" };
    hclib::launch(deps, 1, []() {
        hclib::finish([]() {
            printf("Hello\n");
            hclib::async([&](){ ran = 1; });
        });
    });
    assert(ran == 1);
    printf("Exiting...\n");
    return 0;
}
