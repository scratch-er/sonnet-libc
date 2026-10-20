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

static uint32_t _random_seed;

static uint32_t prand(uint32_t prev) {
    unsigned int feedback = 0;
    feedback ^= (prev>>31) & 1;
    feedback ^= (prev>>21) & 1;
    feedback ^= (prev>>1) & 1;
    feedback ^= (prev>>0) & 1;
    uint32_t current = feedback<<31 | prev>>1;
    return current;
}

int rand(void) {
    if (_random_seed == 0) {
        _random_seed = 0x7a2d5eed;
    } else {
        _random_seed = prand(_random_seed);
    }
    return _random_seed & RAND_MAX;
}

void srand(unsigned int seed) {
    _random_seed = seed;
}

int abs(int x) {
    return (x < 0 ? -x : x);
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
