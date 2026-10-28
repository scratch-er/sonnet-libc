#ifndef __TEST_H
#define __TEST_H

typedef struct test_result_t {
    unsigned int n_ok;
    unsigned int n_fail;
} test_result_t;

typedef test_result_t (*test_func_t)(void);

typedef struct test_def_t {
    const char* name;
    test_func_t func;
} test_def_t;

extern test_def_t test_list[];

#endif
