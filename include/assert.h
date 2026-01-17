#ifndef __ASSERT_H
#define __ASSERT_H

#ifdef NDEBUG

#define assert(ignore) ((void)0)

#else

#include <stdio.h>

#define assert(cond) \
    { \
        if (!(cond)) { \
            printf("Assertion failed at %s:%d\n", __FILE__, __LINE__); \
            halt(1); \
        } \
    }

#endif

#endif /* __ASSERT_H */
