# Sonnet libc

Sonnet libc is a minimal libc for really constrained devices. It was initially designed for SoCs in project One Student One Chip and project ECOS.

## Supported Features and Limitations

Supported:

- Memory functions: `malloc`, `free`, `memcpy`, `memset`
- String functions: `strlen`, `strcpy`, `strcmp`
- Output functions: `printf` (integer only), `putchar`, `puts`

Not Supported:

- Input functions: `scanf`, `getchar`, `gets`.
- Anything related with floating-point
- Anything related with operating systems
- Time/date functions (`<time.h>`)
- Complex number support
- Locale and wide character support

## Building

Simply build this project with `make`. And install the built library along with headers with `make DESTDIR=/path/to/sysroot install`.

You can optionally provide a toolchain file to specify build target and options. Toolchain files in Sonnet SKD are also applicable here. This is an example toolchain file:

```makefile
CC := clang
AR := llvm-ar

CFLAGS += --target=riscv32-unknown-elf -mabi=ilp32 -march=rv32im -ffreestanding -nostdlib -O3 -g
```

## System Functions and Variables

Some libc functions require low level system functions and variables that sonnet libc cannot provide an implementation. If you are using AM or sonnet SDK, these functions and variables are already implemented. If you are porting this libc to your own BSP, you need to implement them on your own if you need to use the libc functions requiring them. The following is a complete list of them.

```c
/* Write a raw character to a low level console. */
/* Required by `stdio.h` output functions. */
void putch(char ch);

/* Represents the range of memory. */
typedef struct {
    void* start; /* starting address (inclusive) */
    void* end;   /* ending address (not inclusive) */
} area_t;

/* The range of memory available for heap. */
/* Required by `malloc`. */
extern const area_t heap;

/* Stopping the processor with a specific code. */
/* Required by `exit`, `abort` and `assert`. */
__attribute__ ((__noreturn__))
void halt(int code);
```

