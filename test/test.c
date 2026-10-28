#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

int main(void) {
    bool has_failure = false;
    for (size_t i=0; test_list[i].func; ++i) {
        test_result_t result = test_list[i].func();
        const char* result_msg;
        if (result.n_fail == 0) {
            result_msg = "[ OK ]";
        } else {
            result_msg = "[FAIL]";
            has_failure = true;
        }
        printf("%s %03u of %03u passed %s\n", result_msg, result.n_ok, result.n_ok+result.n_fail, test_list[i].name);
    }
    return has_failure;
}

#if !defined(__GNUC__) && !defined(__clang__)
#  error "This implementation requires GCC- or Clang-style inline assembly"
#endif

#if !defined(__linux__)
#  error "Tests only support Linux (x86_64, aarch64, riscv64, loongarch64)"
#endif

#if defined(__x86_64__)
/* Linux x86_64: __NR_write = 1, __NR_exit = 60 */
static long sys_write(const char *buf, long n) {
    long ret;
    __asm__ volatile(
        "syscall"
        : "=a"(ret)
        : "a"(1L), "D"(1L), "S"(buf), "d"(n)
        : "rcx", "r11", "memory"
    );
    return ret;
}
static void sys_exit(int code) {
    __asm__ volatile(
        "syscall"
        :
        : "a"(60L), "D"((long)code)
        : "rcx", "r11", "memory"
    );
}
#elif defined(__aarch64__)
/* Linux arm64: __NR_write = 64, __NR_exit = 93, number in x8 */
static long sys_write(const char *buf, long n) {
    register long x0 __asm__("x0") = 1L;
    register long x1 __asm__("x1") = (long)buf;
    register long x2 __asm__("x2") = n;
    register long x8 __asm__("x8") = 64L;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x1), "r"(x2), "r"(x8) : "memory");
    return x0;
}
static void sys_exit(int code) {
    register long x0 __asm__("x0") = (long)code;
    register long x8 __asm__("x8") = 93L;
    __asm__ volatile("svc #0" :: "r"(x0), "r"(x8) : "memory");
}
#elif defined(__riscv) && defined(__riscv_xlen) && __riscv_xlen == 64
/* Linux riscv64: __NR_write = 64, __NR_exit = 93, number in a7 */
static long sys_write(const char *buf, long n) {
    register long a0 __asm__("a0") = 1L;
    register long a1 __asm__("a1") = (long)buf;
    register long a2 __asm__("a2") = n;
    register long a7 __asm__("a7") = 64L;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
    return a0;
}
static void sys_exit(int code) {
    register long a0 __asm__("a0") = (long)code;
    register long a7 __asm__("a7") = 93L;
    __asm__ volatile("ecall" :: "r"(a0), "r"(a7) : "memory");
}
#elif defined(__loongarch64)
/* Linux loongarch64: __NR_write = 64, __NR_exit = 93, number in a7 */
static long sys_write(const char *buf, long n) {
    register long a0 __asm__("a0") = 1L;
    register long a1 __asm__("a1") = (long)buf;
    register long a2 __asm__("a2") = n;
    register long a7 __asm__("a7") = 64L;
    __asm__ volatile("syscall 0" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
    return a0;
}
static void sys_exit(int code) {
    register long a0 __asm__("a0") = (long)code;
    register long a7 __asm__("a7") = 93L;
    __asm__ volatile("syscall 0" :: "r"(a0), "r"(a7) : "memory");
}
#else
#  error "Unsupported architecture"
#endif

/* Platform hooks the libc expects. */

void putch(char ch) {
    sys_write(&ch, 1);
}

__attribute__((noreturn))
void halt(int code) {
    sys_exit(code);
    __builtin_unreachable();
}

/* Empty heap: malloc always fails, which is fine for these tests. */
typedef struct {
    void* start;
    void* end;
} area_t;

const area_t heap = { NULL, NULL };

__attribute__((noreturn))
void _start(void) {
    exit(main());
}
