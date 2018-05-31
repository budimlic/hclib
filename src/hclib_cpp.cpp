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

hclib_worker_state *hclib::current_ws() {
    return CURRENT_WS_INTERNAL;
}

int hclib::get_current_worker() {
    return hclib_get_current_worker();
}

int hclib::get_num_workers() {
    return hclib_get_num_workers();
}

int hclib::get_num_locales() {
    return hclib_get_num_locales();
}

hclib::locale_t *hclib::get_closest_locale() {
    return hclib_get_closest_locale();
}

hclib::locale_t **hclib::get_thread_private_locales() {
    return hclib_get_thread_private_locales();
}

hclib::locale_t *hclib::get_all_locales() {
    return hclib_get_all_locales();
}

hclib_locale_t **hclib::get_all_locales_of_type(int type, int *out_count) {
    return hclib_get_all_locales_of_type(type, out_count);
}

hclib_locale_t *hclib::get_master_place() {
    return hclib_get_master_place();
}

hclib::future_t<void*> *hclib::allocate_at(size_t nbytes, hclib::locale_t *locale) {
    hclib_future_t *actual = hclib_allocate_at(nbytes, locale);
    return (hclib::future_t<void*> *)actual;
}

hclib::future_t<void*> *hclib::reallocate_at(void *ptr, size_t nbytes,
        hclib::locale_t *locale) {
    hclib_future_t *actual = hclib_reallocate_at(ptr, nbytes, locale);
    return (hclib::future_t<void*> *)actual;
}

void hclib::free_at(void *ptr, hclib::locale_t *locale) {
    hclib_free_at(ptr, locale);
}

hclib::future_t<void*> *hclib::memset_at(void *ptr, int pattern, size_t nbytes,
        hclib::locale_t *locale) {
    hclib_future_t *actual = hclib_memset_at(ptr, pattern, nbytes, locale);
    return (hclib::future_t<void*> *)actual;
}
