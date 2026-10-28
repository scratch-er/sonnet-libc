#ifndef __STDLIB_H
#define __STDLIB_H

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

#define RAND_MAX 0x7fffffff

void srand(unsigned int seed);

int rand(void);

void *malloc(size_t size);

void *realloc(void *ptr, size_t new_size);

void *calloc(size_t num, size_t size);

void free(void *ptr);

int abs(int x);

long int labs(long int x);

long long int llabs(long long int x);

int atoi(const char *nptr);

long int atol(const char* nptr);

long long int atoll(const char* nptr);

long int strtol(const char* str, char** endptr, int base);

long long int strtoll(const char* str, char** endptr, int base);

unsigned long int strtoul(const char* str, char** endptr, int base);

unsigned long long int strtoull(const char* str, char** endptr, int base);

void* bsearch(const void* key, const void* base, size_t n, size_t size, int (*cmp)(const void*, const void*));

void qsort(void* base, size_t n, size_t size, int (*cmp)(const void* , const void*));

__attribute__ ((__noreturn__))
void exit(int code);

__attribute__ ((__noreturn__))
void abort(void);

typedef struct div_t {
    int quot;
    int rem;
} div_t;

typedef struct ldiv_t {
    long int quot;
    long int rem;
} ldiv_t;

typedef struct lldiv_t {
    long long int quot;
    long long int rem;
} lldiv_t;

div_t div(int numer, int denom);

ldiv_t ldiv(long int numer, long int denom);

lldiv_t lldiv(long long int numer, long long int denom);

size_t memalignment(const void* p);

#ifdef __cplusplus
}
#endif

#endif /* __STDLIB_H */
