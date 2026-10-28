/* Note: this file is AI generated. */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

static test_result_t test_memset(void) {
    test_result_t r = {0, 0};
    unsigned char buf[512];

    /* fixed cases: {n, c} */
    static const struct { size_t n; int c; } cases[] = {
        {0, 0}, {0, 'a'}, {1, 'x'}, {2, 0}, {3, 0xff}, {7, 'A'},
        {16, 0x100 + 'b'}, /* c truncated to unsigned char */
        {255, 0x7f}, {511, 0x80}, {512, 'z'},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        size_t n = cases[k].n;
        int c = cases[k].c;
        unsigned char val = (unsigned char)c;
        /* poison the buffer first */
        for (size_t i = 0; i < sizeof(buf); ++i) buf[i] = 0xaa;
        void *ret = memset(buf, c, n);
        if (ret != buf) {
            printf("memset: returned %p, expected %p\n", ret, (void*)buf);
            r.n_fail++;
        }
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            if (buf[i] != val) { ok = 0; break; }
        }
        for (size_t i = n; i < sizeof(buf); ++i) {
            if (buf[i] != 0xaa) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("memset: case n=%zu c=%d failed\n", n, c); r.n_fail++; }
    }

    /* random cases */
    srand(12345);
    for (int t = 0; t < 200; ++t) {
        size_t n = (size_t)rand() % sizeof(buf);
        int c = rand() & 0xff;
        memset(buf, c, n);
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            if (buf[i] != (unsigned char)c) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("memset: random case %d failed (n=%zu c=%d)\n", t, n, c); r.n_fail++; }
    }
    return r;
}

static test_result_t test_memcpy(void) {
    test_result_t r = {0, 0};
    unsigned char src[512], dst[512];

    static const size_t cases[] = {0, 1, 2, 3, 7, 16, 100, 255, 511, 512};
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        size_t n = cases[k];
        for (size_t i = 0; i < sizeof(src); ++i) src[i] = (unsigned char)(i * 31 + 7);
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0x55;
        void *ret = memcpy(dst, src, n);
        if (ret != dst) {
            printf("memcpy: returned %p, expected %p\n", ret, (void*)dst);
            r.n_fail++;
        }
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            if (dst[i] != src[i]) { ok = 0; break; }
        }
        for (size_t i = n; i < sizeof(dst); ++i) {
            if (dst[i] != 0x55) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("memcpy: case n=%zu failed\n", n); r.n_fail++; }
    }

    srand(2222);
    for (int t = 0; t < 200; ++t) {
        size_t n = (size_t)rand() % sizeof(src);
        for (size_t i = 0; i < n; ++i) src[i] = (unsigned char)rand();
        memcpy(dst, src, n);
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            if (dst[i] != src[i]) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("memcpy: random case %d failed (n=%zu)\n", t, n); r.n_fail++; }
    }
    return r;
}

static test_result_t test_memmove(void) {
    test_result_t r = {0, 0};
    unsigned char buf[1024], expect[1024];

    /* non-overlapping */
    static const size_t cases[] = {0, 1, 2, 3, 7, 16, 100, 255, 512};
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        size_t n = cases[k];
        for (size_t i = 0; i < sizeof(buf); ++i) buf[i] = (unsigned char)(i * 13 + 5);
        void *ret = memmove(buf + 500, buf + 100, n);
        if (ret != buf + 500) {
            printf("memmove: returned %p, expected %p\n", ret, (void*)(buf + 500));
            r.n_fail++;
        }
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            if (buf[500 + i] != (unsigned char)((100 + i) * 13 + 5)) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("memmove: non-overlap n=%zu failed\n", n); r.n_fail++; }
    }

    /* overlapping: dst > src, try every overlap offset */
    for (size_t off = 1; off < 32; ++off) {
        size_t n = 64;
        for (size_t i = 0; i < sizeof(buf); ++i) buf[i] = (unsigned char)(i * 7 + 3);
        for (size_t i = 0; i < sizeof(expect); ++i) expect[i] = buf[i];
        memmove(buf + off, buf, n);
        /* expected: bytes [off, off+n) = original [0, n), rest untouched */
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            if (buf[off + i] != expect[i]) { ok = 0; break; }
        }
        for (size_t i = 0; i < off; ++i) {
            if (buf[i] != expect[i]) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("memmove: dst>src off=%zu failed\n", off); r.n_fail++; }
    }

    /* overlapping: dst < src, try every overlap offset */
    for (size_t off = 1; off < 32; ++off) {
        size_t n = 64;
        for (size_t i = 0; i < sizeof(buf); ++i) buf[i] = (unsigned char)(i * 7 + 3);
        for (size_t i = 0; i < sizeof(expect); ++i) expect[i] = buf[i];
        memmove(buf, buf + off, n);
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            if (buf[i] != expect[off + i]) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("memmove: dst<src off=%zu failed\n", off); r.n_fail++; }
    }

    /* random overlapping moves */
    srand(777);
    for (int t = 0; t < 300; ++t) {
        size_t n = 1 + (size_t)rand() % 200;
        size_t s = (size_t)rand() % (sizeof(buf) - n);
        size_t d = (size_t)rand() % (sizeof(buf) - n);
        for (size_t i = 0; i < sizeof(buf); ++i) buf[i] = (unsigned char)rand();
        for (size_t i = 0; i < sizeof(expect); ++i) expect[i] = buf[i];
        memmove(buf + d, buf + s, n);
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            if (buf[d + i] != expect[s + i]) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("memmove: random case %d failed (d=%zu s=%zu n=%zu)\n", t, d, s, n); r.n_fail++; }
    }
    return r;
}

static test_result_t test_memcmp(void) {
    test_result_t r = {0, 0};
    unsigned char a[256], b[256];

    /* n = 0 is always equal */
    if (memcmp(a, b, 0) == 0) r.n_ok++; else { printf("memcmp: n=0 not equal\n"); r.n_fail++; }

    static const size_t cases[] = {1, 2, 3, 7, 16, 100, 255, 256};
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        size_t n = cases[k];
        for (size_t i = 0; i < sizeof(a); ++i) { a[i] = (unsigned char)i; b[i] = (unsigned char)i; }
        if (memcmp(a, b, n) == 0) r.n_ok++; else { printf("memcmp: equal case n=%zu failed\n", n); r.n_fail++; }

        /* first byte greater: a[0]=1 > b[0]=0 */
        b[0] = 0; a[0] = 1;
        if (memcmp(a, b, n) == 1) r.n_ok++; else { printf("memcmp: gt-first n=%zu failed\n", n); r.n_fail++; }

        /* first byte less */
        a[0] = 0; b[0] = 1;
        if (memcmp(a, b, n) == -1) r.n_ok++; else { printf("memcmp: lt-first n=%zu failed\n", n); r.n_fail++; }

        /* difference at the last byte of the range, bytes before equal */
        for (size_t i = 0; i < sizeof(a); ++i) { a[i] = 5; b[i] = 5; }
        a[n - 1] = 9;
        if (memcmp(a, b, n) == 1) r.n_ok++; else { printf("memcmp: gt-last n=%zu failed\n", n); r.n_fail++; }

        /* difference beyond the range must not matter */
        for (size_t i = 0; i < sizeof(a); ++i) { a[i] = 5; b[i] = 5; }
        if (n < sizeof(a)) {
            a[n] = 200;
            if (memcmp(a, b, n) == 0) r.n_ok++; else { printf("memcmp: beyond-range n=%zu failed\n", n); r.n_fail++; }
        } else {
            r.n_ok++;
        }
    }

    /* unsigned comparison: 0xff > 0x01 */
    a[0] = 0xff; b[0] = 0x01;
    if (memcmp(a, b, 1) == 1) r.n_ok++; else { printf("memcmp: unsigned gt failed\n"); r.n_fail++; }
    a[0] = 0x01; b[0] = 0xff;
    if (memcmp(a, b, 1) == -1) r.n_ok++; else { printf("memcmp: unsigned lt failed\n"); r.n_fail++; }

    /* random equal buffers */
    srand(999);
    for (int t = 0; t < 200; ++t) {
        size_t n = 1 + (size_t)rand() % sizeof(a);
        for (size_t i = 0; i < n; ++i) { a[i] = (unsigned char)rand(); b[i] = a[i]; }
        if (memcmp(a, b, n) == 0) r.n_ok++; else { printf("memcmp: random equal %d failed\n", t); r.n_fail++; }
    }
    return r;
}

static test_result_t test_memchr(void) {
    test_result_t r = {0, 0};
    unsigned char buf[256];

    for (size_t i = 0; i < sizeof(buf); ++i) buf[i] = (unsigned char)i;

    static const struct { size_t n; int c; } cases[] = {
        {0, 0}, {0, 'x'}, {1, 0}, {1, 1}, {256, 0}, {256, 255},
        {10, 9}, {10, 10},      /* c at boundary position n (not found) */
        {100, 99}, {100, 100},  /* found / just past */
        {256, 0x100 + 5},       /* c truncated to unsigned char */
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        size_t n = cases[k].n;
        int c = cases[k].c;
        /* reference first index < n where buf[i] == (unsigned char)c */
        long expect_pos = -1;
        for (size_t i = 0; i < n; ++i) {
            if (buf[i] == (unsigned char)c) { expect_pos = (long)i; break; }
        }
        void *got = memchr(buf, c, n);
        int ok = (expect_pos < 0) ? (got == NULL) : (got == (void*)(buf + expect_pos));
        if (ok) r.n_ok++;
        else { printf("memchr: n=%zu c=%d -> %p, expected offset %ld\n", n, c, got, expect_pos); r.n_fail++; }
    }

    /* values not present at all */
    {
        unsigned char zeros[64] = {0};
        if (memchr(zeros, 1, 64) == NULL) r.n_ok++;
        else { printf("memchr: absent-in-zeros failed\n"); r.n_fail++; }
    }

    /* random */
    srand(1212);
    for (int t = 0; t < 500; ++t) {
        size_t n = (size_t)rand() % sizeof(buf);
        for (size_t i = 0; i < n; ++i) buf[i] = (unsigned char)rand();
        int c = rand() & 0xff;
        long expect_pos = -1;
        for (size_t i = 0; i < n; ++i) {
            if (buf[i] == (unsigned char)c) { expect_pos = (long)i; break; }
        }
        void *got = memchr(buf, c, n);
        int ok = (expect_pos < 0) ? (got == NULL) : (got == (void*)(buf + expect_pos));
        if (ok) r.n_ok++; else { printf("memchr: random %d failed (n=%zu c=%d)\n", t, n, c); r.n_fail++; }
    }
    return r;
}

test_def_t test_list[] = {
    {"memset", test_memset},
    {"memcpy", test_memcpy},
    {"memmove", test_memmove},
    {"memcmp", test_memcmp},
    {"memchr", test_memchr},
    {NULL, NULL},
};
