#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdalign.h>

typedef struct {
    void* start;
    void* end;
} area_t;

extern const area_t heap;

__attribute__ ((__noreturn__))
void halt(int code);

static uint32_t __random_seed;

static uint32_t __prand(uint32_t prev) {
    unsigned int feedback = 0;
    feedback ^= (prev>>31) & 1;
    feedback ^= (prev>>21) & 1;
    feedback ^= (prev>>1) & 1;
    feedback ^= (prev>>0) & 1;
    uint32_t current = feedback<<31 | prev>>1;
    return current;
}

int rand(void) {
    if (__random_seed == 0) {
        __random_seed = 0x7a2d5eed;
    } else {
        __random_seed = __prand(__random_seed);
    }
    return __random_seed & RAND_MAX;
}

void srand(unsigned int seed) {
    __random_seed = seed;
}

int abs(int x) {
    return (x < 0 ? -x : x);
}

long int labs(long int x) {
    return (x < 0 ? -x : x);
}

long long int llabs(long long int x) {
    return (x < 0 ? -x : x);
}

div_t div(int numer, int denom) {
    div_t result;
    result.quot = numer / denom;
    result.rem = numer % denom;
    return result;
}

ldiv_t ldiv(long int numer, long int denom) {
    ldiv_t result;
    result.quot = numer / denom;
    result.rem = numer % denom;
    return result;
}

lldiv_t lldiv(long long int numer, long long int denom) {
    lldiv_t result;
    result.quot = numer / denom;
    result.rem = numer % denom;
    return result;
}

int atoi(const char* nptr) {
    int x = 0;
    bool negative = false;
    while (isspace(*nptr)) {
        nptr++;
    }
    if (*nptr == '+') {
        negative = false;
        ++nptr;
    } else if (*nptr == '-') {
        negative = true;
        ++nptr;
    }
    while (*nptr >= '0' && *nptr <= '9') {
        x = x * 10 + *nptr - '0';
        nptr ++;
    }
    if (negative) {
        return -x;
    } else {
        return x;
    }
}

long int atol(const char* nptr) {
    long int x = 0;
    bool negative = false;
    while (isspace(*nptr)) {
        nptr++;
    }
    if (*nptr == '+') {
        negative = false;
        ++nptr;
    } else if (*nptr == '-') {
        negative = true;
        ++nptr;
    }
    while (*nptr >= '0' && *nptr <= '9') {
        x = x * 10 + *nptr - '0';
        nptr ++;
    }
    if (negative) {
        return -x;
    } else {
        return x;
    }
}

long long int atoll(const char* nptr) {
    long long int x = 0;
    bool negative = false;
    while (isspace(*nptr)) {
        nptr++;
    }
    if (*nptr == '+') {
        negative = false;
        ++nptr;
    } else if (*nptr == '-') {
        negative = true;
        ++nptr;
    }
    while (*nptr >= '0' && *nptr <= '9') {
        x = x * 10 + *nptr - '0';
        nptr ++;
    }
    if (negative) {
        return -x;
    } else {
        return x;
    }
}

long int strtol(const char* str, char** endptr, int base) {
    size_t i = 0;
    bool negative = false;
    long int result = 0;
    // fail if `base` is invalid
    if (base!=0 && (base<2 || base>36)) {
        goto finish;
    }
    // skip all whitespaces
    while (isspace(str[i])) {
        ++i;
    }
    // detect sign
    if (str[i] == '+') {
        negative = false;
        ++i;
    } else if (str[i] == '-') {
        negative = true;
        ++i;
    }
    // detect base
    if (base == 0) {
        if (str[i] == '0') {
            ++i;
            if (str[i]=='x' || str[i]=='X') {
                base = 16;
                ++i;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    } else if (base == 16) {
        // sikp the "0x" prefix for hexadecimal
        if (str[i] == '0') {
            ++i;
        }
        if (str[i]=='x' || str[i]=='X') {
            ++i;
        }
    }
    // parse number
    while (true) {
        long int digit;
        if (isdigit(str[i])) {
            digit = str[i] - '0';
        } else if (isupper(str[i])) {
            digit = str[i] - 'A' + 10;
        } else if (islower(str[i])) {
            digit = str[i] - 'a' + 10;
        } else {
            break;
        }
        if (digit >= base) {
            break;
        }
        result *= base;
        result += digit;
        ++i;
    }
    finish:
    if (endptr != NULL) {
        *endptr = (char*)str + i;
    }
    if (negative) {
        return -result;
    } else {
        return result;
    }
}

long long int strtoll(const char* str, char** endptr, int base) {
    size_t i = 0;
    bool negative = false;
    long long int result = 0;
    // fail if `base` is invalid
    if (base!=0 && (base<2 || base>36)) {
        goto finish;
    }
    // skip all whitespaces
    while (isspace(str[i])) {
        ++i;
    }
    // detect sign
    if (str[i] == '+') {
        negative = false;
        ++i;
    } else if (str[i] == '-') {
        negative = true;
        ++i;
    }
    // detect base
    if (base == 0) {
        if (str[i] == '0') {
            ++i;
            if (str[i]=='x' || str[i]=='X') {
                base = 16;
                ++i;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    } else if (base == 16) {
        // sikp the "0x" prefix for hexadecimal
        if (str[i] == '0') {
            ++i;
        }
        if (str[i]=='x' || str[i]=='X') {
            ++i;
        }
    }
    // parse number
    while (true) {
        long int digit;
        if (isdigit(str[i])) {
            digit = str[i] - '0';
        } else if (isupper(str[i])) {
            digit = str[i] - 'A' + 10;
        } else if (islower(str[i])) {
            digit = str[i] - 'a' + 10;
        } else {
            break;
        }
        if (digit >= base) {
            break;
        }
        result *= base;
        result += digit;
        ++i;
    }
    finish:
    if (endptr != NULL) {
        *endptr = (char*)str + i;
    }
    if (negative) {
        return -result;
    } else {
        return result;
    }
}

unsigned long int strtoul(const char* str, char** endptr, int base) {
    size_t i = 0;
    unsigned long int result = 0;
    // fail if `base` is invalid
    if (base!=0 && (base<2 || base>36)) {
        goto finish;
    }
    // skip all whitespaces
    while (isspace(str[i])) {
        ++i;
    }
    // detect base
    if (base == 0) {
        if (str[i] == '0') {
            ++i;
            if (str[i]=='x' || str[i]=='X') {
                base = 16;
                ++i;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    } else if (base == 16) {
        // sikp the "0x" prefix for hexadecimal
        if (str[i] == '0') {
            ++i;
        }
        if (str[i]=='x' || str[i]=='X') {
            ++i;
        }
    }
    // parse number
    while (true) {
        long int digit;
        if (isdigit(str[i])) {
            digit = str[i] - '0';
        } else if (isupper(str[i])) {
            digit = str[i] - 'A' + 10;
        } else if (islower(str[i])) {
            digit = str[i] - 'a' + 10;
        } else {
            break;
        }
        if (digit >= base) {
            break;
        }
        result *= base;
        result += digit;
        ++i;
    }
    finish:
    if (endptr != NULL) {
        *endptr = (char*)str + i;
    }
    return result;
}

unsigned long long int strtoull(const char* str, char** endptr, int base) {
    size_t i = 0;
    unsigned long long int result = 0;
    // fail if `base` is invalid
    if (base!=0 && (base<2 || base>36)) {
        goto finish;
    }
    // skip all whitespaces
    while (isspace(str[i])) {
        ++i;
    }
    // detect base
    if (base == 0) {
        if (str[i] == '0') {
            ++i;
            if (str[i]=='x' || str[i]=='X') {
                base = 16;
                ++i;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    } else if (base == 16) {
        // sikp the "0x" prefix for hexadecimal
        if (str[i] == '0') {
            ++i;
        }
        if (str[i]=='x' || str[i]=='X') {
            ++i;
        }
    }
    // parse number
    while (true) {
        long int digit;
        if (isdigit(str[i])) {
            digit = str[i] - '0';
        } else if (isupper(str[i])) {
            digit = str[i] - 'A' + 10;
        } else if (islower(str[i])) {
            digit = str[i] - 'a' + 10;
        } else {
            break;
        }
        if (digit >= base) {
            break;
        }
        result *= base;
        result += digit;
        ++i;
    }
    finish:
    if (endptr != NULL) {
        *endptr = (char*)str + i;
    }
    return result;
}

void *malloc(size_t size) {
    static uintptr_t ptr;
    static bool initialized;
    // initialize ptr
    if (!initialized) {
        ptr = (uintptr_t)heap.start;
        initialized = true;
    }
    // align ptr to a higher address
    uintptr_t aligned_ptr = ptr;
    if (aligned_ptr%alignof(max_align_t) != 0) {
        aligned_ptr += alignof(max_align_t) - aligned_ptr%alignof(max_align_t);
    }
    // return NULL if fully allocated
    if (size > (uintptr_t)heap.end - aligned_ptr) {
        return NULL;
    }
    ptr = aligned_ptr + size;
    return (void*)aligned_ptr;
}

void free(void *ptr) {
    return;
}

__attribute__ ((__noreturn__))
void exit(int code) {
    halt(code);
}

__attribute__ ((__noreturn__))
void abort(void) {
    halt(-1);
}

void* bsearch(const void* key, const void* base, size_t n, size_t size, int (*cmp)(const void*, const void*)) {
    const unsigned char* base_c = (const unsigned char*)base;
    size_t begin = 0;
    size_t end = n;
    while (begin < end) {
        size_t mid = begin + (end - begin) / 2;
        const char* mid_ptr = (const char*)(base_c+mid*size);
        int cmp_result = cmp(key, mid_ptr);
        if (cmp_result == 0) {
            return (void*)mid_ptr;
        } else if (cmp_result < 0) {
            end = mid;
        } else if (cmp_result > 0) {
            begin = mid + 1;
        }
    }
    return NULL;
}

static void __swap(void* a, void* b, size_t size) {
    // we cannot assume the alignment of a and b
    // use byte by byte swap here
    for (size_t i=0; i<size; ++i) {
        unsigned char tmp;
        tmp = ((unsigned char*)a)[i];
        ((unsigned char*)a)[i] = ((unsigned char*)b)[i];
        ((unsigned char*)b)[i] = tmp;
    }
}

void qsort(void* base, size_t n, size_t size, int (*cmp)(const void* , const void*)) {
    unsigned char* base_c = (unsigned char*)base;
    // byte by byte swap is slow
    // use selection sort for smaller input
    if (n <= 16) {
        size_t i = 0;
        size_t count_i = 0;
        while (count_i+1 < n) {
            size_t min_j = i;
            size_t j = i + size;
            size_t count_j = count_i + 1;
            while (count_j < n) {
                if (cmp(base_c+j, base_c+min_j) < 0) {
                    min_j = j;
                }
                j += size;
                count_j += 1;
            }
            __swap(base_c+i, base_c+min_j, size);
            i += size;
            count_i += 1;
        }
    } else {
        size_t i = 0;
        size_t count_i = 0;
        size_t j = size;
        size_t count_j = 1;
        // using the beginning element as pivot
        while (count_j < n) {
            if (cmp(base_c+j, base_c) < 0) {
                i += size;
                count_i += 1;
                __swap(base_c+i, base_c+j, size);
            }
            j += size;
            count_j += 1;
        }
        __swap(base_c, base_c+i, size);
        qsort(base_c, count_i, size, cmp);
        qsort(base_c+i+size, n-count_i-1, size, cmp);
    }
}
