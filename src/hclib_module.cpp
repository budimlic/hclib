#include "hclib-module.h"

#include <algorithm>
#include <assert.h>
#include <vector>
#include <iostream>

static std::vector<hclib_module_pre_init_func_type> *pre_init_functions =
        NULL;
static std::vector<hclib_module_pre_init_func_type> *post_init_functions =
        NULL;
static std::vector<hclib_module_finalize_func_type> *finalize_functions = NULL;

#ifdef __cplusplus
extern "C" {
#endif

int hclib_add_module_init_function(const char *name,
        hclib_module_pre_init_func_type pre,
        hclib_module_post_init_func_type post,
        hclib_module_finalize_func_type finalize) {
    std::cout << "Registering module " << name << std::endl;

    if (pre_init_functions == NULL) {
        pre_init_functions = new std::vector<hclib_module_pre_init_func_type>();
        post_init_functions =
            new std::vector<hclib_module_post_init_func_type>();
        finalize_functions = new std::vector<hclib_module_finalize_func_type>();
    }

    if (std::find(pre_init_functions->begin(), pre_init_functions->end(),
                pre) != pre_init_functions->end()) {
        assert(std::find(post_init_functions->begin(),
                    post_init_functions->end(), post) !=
                post_init_functions->end());
        assert(std::find(finalize_functions->begin(), finalize_functions->end(),
                    finalize) != finalize_functions->end());
    } else {
        assert(std::find(post_init_functions->begin(),
                    post_init_functions->end(), post) ==
                post_init_functions->end());
        assert(std::find(finalize_functions->begin(), finalize_functions->end(),
                    finalize) == finalize_functions->end());

        pre_init_functions->push_back(pre);
        post_init_functions->push_back(post);
        finalize_functions->push_back(finalize);
    }

    return 0;
}

void hclib_call_module_pre_init_functions() {
    if (pre_init_functions == NULL) {
        std::cout << "Pre-initializing 0 module(s)" << std::endl;
        return;
    }

    std::cout << "Pre-initializing " << pre_init_functions->size() <<
        " module(s)" << std::endl;
    for (std::vector<hclib_module_pre_init_func_type>::iterator i =
            pre_init_functions->begin(), e =
            pre_init_functions->end(); i != e; i++) {
        hclib_module_pre_init_func_type curr = *i;
        (*curr)();
    }
}

void hclib_call_module_post_init_functions() {
    if (post_init_functions == NULL) {
        std::cout << "Post-initializing 0 module(s)" << std::endl;
        return;
    }

    std::cout << "Post-initializing " << post_init_functions->size() <<
        " module(s)" << std::endl;
    for (std::vector<hclib_module_post_init_func_type>::iterator i =
            post_init_functions->begin(), e =
            post_init_functions->end(); i != e; i++) {
        hclib_module_post_init_func_type curr = *i;
        (*curr)();
    }
}

void hclib_call_finalize_functions() {
    if (finalize_functions == NULL) {
        std::cout << "Finalizing 0 module(s)" << std::endl;
        return;
    }

    std::cout << "Finalizing " << finalize_functions->size() << " module(s)" <<
        std::endl;
    for (std::vector<hclib_module_finalize_func_type>::iterator i =
            finalize_functions->begin(), e = finalize_functions->end(); i != e;
            i++) {
        hclib_module_finalize_func_type curr = *i;
        (*curr)();
    }
}

#ifdef __cplusplus
}
#endif