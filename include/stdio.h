#ifndef __STDIO_H
#define __STDIO_H

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void putch(char ch);

int scanf(const char *format, ...);

int sscanf(const char *s, const char *format, ...);

int vscanf(const char *format, va_list arg);

int vsscanf(const char * s, const char * format, va_list arg);

int printf(const char *format, ...);

int sprintf(char *str, const char *format, ...);

int snprintf(char *str, size_t size, const char *format, ...);

int vsprintf(char *str, const char *format, va_list ap);

int vsnprintf(char *str, size_t size, const char *format, va_list ap);

int putchar(int c);

int puts(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* __STDIO_H */
