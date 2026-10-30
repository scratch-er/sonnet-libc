#ifndef __STRING_H
#define __STRING_H

#define __STDC_VERSION_STRING_H__ 202311L

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *memset(void *s, int c, size_t n);

void *memcpy(void *dst, const void *src, size_t n);

void *memmove(void *dst, const void *src, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);

size_t strlen(const char *s);

char *strcat(char *dst, const char *src);

char *strncat(char *dst, const char *src, size_t count);

char *strcpy(char *dst, const char *src);

char *strncpy(char *dst, const char *src, size_t n);

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);

char *strchr(const char *s, int c);

char *strrchr(const char *s, int c);

char *strstr(const char *haystack, const char *needle);

size_t strspn(const char *s, const char *accept);

size_t strcspn(const char *s, const char *reject);

char *strpbrk(const char *s, const char *accept);

void *memchr(const void *s, int c, size_t n);

#if __STDC_VERSION__ >= 202311L

#define strchr(s, c) _Generic((s), \
    char*: strchr((s), (c)), \
    const char*: ((const char*)strchr((s), (c))) \
)

#define strrchr(s, c) _Generic((s), \
    char*: strrchr((s), (c)), \
    const char*: ((const char*)strrchr((s), (c))) \
)

#define strstr(s, c) _Generic((s), \
    char*: strstr((s), (c)), \
    const char*: ((const char*)strstr((s), (c))) \
)

#define memchr(p, c, n) __builtin_choose_expr( \
    __builtin_types_compatible_p(__typeof__(p), const __typeof__(*(p))*), \
    (const void*)memchr((p), (c), (n)), \
    memchr((p), (c), (n)))

#endif /* __STDC_VERSION__ >= 202311L */

#ifdef __cplusplus
}
#endif

#endif
