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

#include "hclib.hpp"
#include "hclib-place.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    const char *deps[] = { "system" };
    hclib::launch(deps, 1, [] {
        hclib::finish([] {

            int numWorkers = hclib::get_num_workers();
            cout << "Total Workers: " << numWorkers << endl;

            int num_locales = hclib::get_num_locales();
            hclib::locale_t *locales = hclib::get_all_locales();

            for (int i = 0; i < num_locales; i++) {
                hclib::locale_t *locale = locales + i;

                hclib::async_at([=] {
                    cerr << "Hello I'm Worker " << hclib::get_current_worker() << " of " << numWorkers << " workers" << endl;
                }, locale);
            }
        });
    });

    return 0;
}
