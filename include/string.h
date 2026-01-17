#ifndef __STRING_H
#define __STRING_H

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

char *strcpy(char *dst, const char *src);

char *strncpy(char *dst, const char *src, size_t n);

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif
