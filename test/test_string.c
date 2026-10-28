/* Note: this file is AI generated. */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

/* ---- shared input generators ---- */

/* length of a string literal, without calling strlen */
#define LITLEN(lit) (sizeof(lit) - 1)

/* fill buf with len random chars drawn from ['a', 'a'+alphabet), excluding
 * any char equal to excl, then NUL-terminate. excl == 0 disables exclusion. */
static void rand_str_excl(char *buf, size_t len, int alphabet, char excl) {
    for (size_t i = 0; i < len; ++i) {
        char c;
        do { c = (char)('a' + rand() % alphabet); } while (c == excl);
        buf[i] = c;
    }
    buf[len] = 0;
}

/* 1 if c is in the NUL-terminated set */
static int in_set(char c, const char *set) {
    for (size_t j = 0; set[j]; ++j) {
        if (set[j] == c) return 1;
    }
    return 0;
}

static test_result_t test_strlen(void) {
    test_result_t r = {0, 0};

    static const char *cases[] = {
        "", "a", "ab", "hello", "hello world",
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    };
    /* expected lengths computed at compile time, no strlen involved */
    static const size_t expects[] = {0, 1, 2, 5, 11, 80};
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        size_t got = strlen(cases[k]);
        if (got == expects[k]) r.n_ok++;
        else { printf("strlen: \"%s\" -> %zu, expected %zu\n", cases[k], got, expects[k]); r.n_fail++; }
    }

    /* random strings: the generated length is the expected answer */
    srand(31337);
    for (int t = 0; t < 500; ++t) {
        char buf[300];
        size_t len = (size_t)rand() % 299;
        rand_str_excl(buf, len, 26, 0);
        size_t got = strlen(buf);
        if (got == len) r.n_ok++; else { printf("strlen: random %d failed (len=%zu got=%zu)\n", t, len, got); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strcpy(void) {
    test_result_t r = {0, 0};
    char dst[512];

    static const char *cases[] = {"", "a", "ab", "hello", "hello world"};
    static const size_t lens[] = {0, 1, 2, 5, 11};
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        const char *src = cases[k];
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0x7f;
        char *ret = strcpy(dst, src);
        if (ret != dst) {
            printf("strcpy: returned %p, expected %p\n", (void*)ret, (void*)dst);
            r.n_fail++;
        }
        /* runtime check of the copy: every byte including the NUL */
        int ok = 1;
        for (size_t i = 0; i <= lens[k]; ++i) {
            if (dst[i] != src[i]) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("strcpy: case \"%s\" failed\n", src); r.n_fail++; }
    }

    /* random strings */
    srand(4242);
    for (int t = 0; t < 500; ++t) {
        char src[300];
        size_t len = (size_t)rand() % 299;
        for (size_t i = 0; i < len; ++i) src[i] = (char)(32 + rand() % 95);
        src[len] = 0;
        strcpy(dst, src);
        int ok = 1;
        for (size_t i = 0; i <= len; ++i) {
            if (dst[i] != src[i]) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("strcpy: random %d failed\n", t); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strncpy(void) {
    test_result_t r = {0, 0};
    char dst[512];

    /* src shorter than n: copy src then pad with zeros */
    {
        const char *src = "abc";
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0x55;
        char *ret = strncpy(dst, src, 10);
        if (ret != dst) { printf("strncpy: bad return\n"); r.n_fail++; }
        int ok = 1;
        for (int i = 0; i < 10; ++i) {
            unsigned char expect = (i < 3) ? (unsigned char)src[i] : 0;
            if ((unsigned char)dst[i] != expect) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("strncpy: pad case failed\n"); r.n_fail++; }
    }

    /* src longer than n: truncate, no NUL written */
    {
        const char *src = "abcdefghij";
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0x55;
        strncpy(dst, src, 4);
        int ok = 1;
        for (int i = 0; i < 4; ++i) {
            if (dst[i] != src[i]) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("strncpy: truncate case failed\n"); r.n_fail++; }
    }

    /* n = 0: nothing written */
    {
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0x55;
        strncpy(dst, "abc", 0);
        int ok = 1;
        for (size_t i = 0; i < 16; ++i) {
            if ((unsigned char)dst[i] != 0x55) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("strncpy: n=0 case failed\n"); r.n_fail++; }
    }

    /* empty src */
    {
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0x55;
        strncpy(dst, "", 5);
        int ok = 1;
        for (int i = 0; i < 5; ++i) {
            if (dst[i] != 0) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("strncpy: empty src case failed\n"); r.n_fail++; }
    }

    /* random: verify exact region [0, n) against expectation */
    srand(555);
    for (int t = 0; t < 500; ++t) {
        char src[64];
        size_t srclen = (size_t)rand() % 40;
        rand_str_excl(src, srclen, 26, 0);
        size_t n = (size_t)rand() % 40;
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0x55;
        strncpy(dst, src, n);
        int ok = 1;
        for (size_t i = 0; i < n; ++i) {
            unsigned char expect = (i < srclen) ? (unsigned char)src[i] : 0;
            if ((unsigned char)dst[i] != expect) { ok = 0; break; }
        }
        if (ok) r.n_ok++; else { printf("strncpy: random %d failed (srclen=%zu n=%zu)\n", t, srclen, n); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strcat(void) {
    test_result_t r = {0, 0};
    char dst[600];

    static const struct { const char *a, *b; size_t la, lb; } cases[] = {
        {"", "", 0, 0}, {"", "x", 0, 1}, {"a", "", 1, 0}, {"abc", "def", 3, 3},
        {"hello", " world", 5, 6}, {"ab", "cd", 2, 2},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        const char *a = cases[k].a, *b = cases[k].b;
        size_t la = cases[k].la, lb = cases[k].lb;
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0x7f;
        for (size_t i = 0; i < la; ++i) dst[i] = a[i];
        dst[la] = 0;
        char *ret = strcat(dst, b);
        if (ret != dst) { printf("strcat: bad return\n"); r.n_fail++; }
        int ok = 1;
        for (size_t i = 0; i < la + lb; ++i) {
            char expect = (i < la) ? a[i] : b[i - la];
            if (dst[i] != expect) { ok = 0; break; }
        }
        if (dst[la + lb] != 0) ok = 0;
        if (ok) r.n_ok++; else { printf("strcat: case \"%s\"+\"%s\" failed\n", a, b); r.n_fail++; }
    }

    /* random: dst starts as a known prefix of src, so the expected result
     * (prefix + src) is known without any libc call */
    srand(808);
    for (int t = 0; t < 200; ++t) {
        char src[128];
        size_t lb = (size_t)rand() % 50;
        rand_str_excl(src, lb, 26, 0);
        size_t la = (size_t)rand() % 50;
        size_t used = (la < lb) ? la : lb;
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0;
        for (size_t i = 0; i < used; ++i) dst[i] = src[i];
        dst[used] = 0;
        strcat(dst, src);
        int ok = 1;
        for (size_t i = 0; i < lb; ++i) {
            if (dst[used + i] != src[i]) { ok = 0; break; }
        }
        if (dst[used + lb] != 0) ok = 0;
        if (ok) r.n_ok++; else { printf("strcat: random %d failed\n", t); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strncat(void) {
    test_result_t r = {0, 0};
    char dst[600];

    static const struct { const char *a, *b; size_t n; size_t la, lb; } cases[] = {
        {"", "", 0, 0, 0}, {"", "x", 0, 0, 1}, {"a", "", 5, 1, 0},
        {"abc", "def", 0, 3, 3}, {"abc", "def", 1, 3, 3}, {"abc", "def", 2, 3, 3},
        {"abc", "def", 3, 3, 3},
        {"abc", "def", 5, 3, 3},        /* n > strlen(b): stop at NUL */
        {"ab", "cdefgh", 4, 2, 6},      /* n < strlen(b): truncate */
        {"hello", " world", 6, 5, 6},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        const char *a = cases[k].a, *b = cases[k].b;
        size_t n = cases[k].n;
        size_t la = cases[k].la, lb = cases[k].lb;
        size_t take = (n < lb) ? n : lb;   /* chars actually appended */
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0;
        for (size_t i = 0; i < la; ++i) dst[i] = a[i];
        dst[la] = 0;
        char *ret = strncat(dst, b, n);
        if (ret != dst) { printf("strncat: bad return\n"); r.n_fail++; }
        int ok = 1;
        for (size_t i = 0; i < take; ++i) {
            if (dst[la + i] != b[i]) { ok = 0; break; }
        }
        if (dst[la + take] != 0) ok = 0;
        /* head unchanged */
        for (size_t i = 0; ok && i < la; ++i) {
            if (dst[i] != a[i]) ok = 0;
        }
        if (ok) r.n_ok++; else { printf("strncat: case \"%s\"+\"%s\" n=%zu failed\n", a, b, n); r.n_fail++; }
    }

    /* random: append a known prefix of src, so dst must end up as
     * (original head) + (that prefix) */
    srand(606);
    for (int t = 0; t < 300; ++t) {
        char src[128];
        size_t lb = (size_t)rand() % 100;
        rand_str_excl(src, lb, 26, 0);
        size_t la = (size_t)rand() % 100;
        size_t n = (size_t)rand() % 120;
        /* expected appended prefix = src[0, min(n, lb)) */
        size_t take = (n < lb) ? n : lb;
        for (size_t i = 0; i < sizeof(dst); ++i) dst[i] = 0;
        for (size_t i = 0; i < la; ++i) dst[i] = (char)('A' + i % 26);
        dst[la] = 0;
        strncat(dst, src, n);
        int ok = dst[la + take] == 0;
        for (size_t i = 0; ok && i < take; ++i) {
            if (dst[la + i] != src[i]) ok = 0;
        }
        if (ok) r.n_ok++; else { printf("strncat: random %d failed (la=%zu lb=%zu n=%zu)\n", t, la, lb, n); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strcmp(void) {
    test_result_t r = {0, 0};

    static const struct { const char *a, *b; int expect; } cases[] = {
        {"", "", 0},
        {"", "a", -1},
        {"a", "", 1},
        {"abc", "abc", 0},
        {"abc", "abd", -1},
        {"abd", "abc", 1},
        {"abc", "ab", 1},       /* prefix: longer string is greater */
        {"ab", "abc", -1},
        {"a", "b", -1},
        {"b", "a", 1},
        {"hello", "hello", 0},
        {"aa", "ab", -1},
        {"za", "zb", -1},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        int got = strcmp(cases[k].a, cases[k].b);
        if (got == cases[k].expect) r.n_ok++;
        else { printf("strcmp: \"%s\" vs \"%s\" -> %d, expected %d\n", cases[k].a, cases[k].b, got, cases[k].expect); r.n_fail++; }
    }

    /* unsigned char comparison: "\xff" > "\x01", and "\xff" > "a" */
    if (strcmp("\xff", "\x01") == 1) r.n_ok++; else { printf("strcmp: unsigned 0xff vs 0x01 failed\n"); r.n_fail++; }
    if (strcmp("\x01", "\xff") == -1) r.n_ok++; else { printf("strcmp: unsigned 0x01 vs 0xff failed\n"); r.n_fail++; }
    if (strcmp("\xff", "a") == 1) r.n_ok++; else { printf("strcmp: unsigned 0xff vs 'a' failed\n"); r.n_fail++; }

    /* antisymmetry on random strings */
    srand(2024);
    for (int t = 0; t < 500; ++t) {
        char a[64], b[64];
        size_t la = (size_t)rand() % 40;
        size_t lb = (size_t)rand() % 40;
        for (size_t i = 0; i < la; ++i) a[i] = (char)(rand() % 4 ? 'a' + rand() % 26 : rand() % 256);
        a[la] = 0;
        for (size_t i = 0; i < lb; ++i) b[i] = (char)(rand() % 4 ? 'a' + rand() % 26 : rand() % 256);
        b[lb] = 0;
        int ab = strcmp(a, b);
        int ba = strcmp(b, a);
        int ok = (ab == -ba) && (ab == -1 || ab == 0 || ab == 1);
        if (ab == 0 && la != lb) ok = 0;
        if (ok) r.n_ok++; else { printf("strcmp: antisymmetry %d failed (\"%s\" vs \"%s\")\n", t, a, b); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strncmp(void) {
    test_result_t r = {0, 0};

    static const struct { const char *a, *b; size_t n; int expect; } cases[] = {
        {"", "", 0, 0},
        {"", "", 5, 0},
        {"", "a", 0, 0},        /* n=0: always equal */
        {"", "a", 1, -1},
        {"a", "", 1, 1},
        {"abc", "abc", 3, 0},
        {"abc", "abc", 2, 0},   /* difference after n must not matter */
        {"abc", "abd", 2, 0},
        {"abc", "abd", 3, -1},
        {"abd", "abc", 3, 1},
        {"abc", "ab", 3, 1},    /* NUL before n terminates */
        {"ab", "abc", 3, -1},
        {"abcdef", "abcxyz", 3, 0},
        {"abcdef", "abcxyz", 4, -1},
        {"hello", "world", 5, -1},
        {"same", "same", 100, 0},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        int got = strncmp(cases[k].a, cases[k].b, cases[k].n);
        if (got == cases[k].expect) r.n_ok++;
        else { printf("strncmp: \"%s\" vs \"%s\" n=%zu -> %d, expected %d\n",
                      cases[k].a, cases[k].b, cases[k].n, got, cases[k].expect); r.n_fail++; }
    }

    /* unsigned comparison within first n bytes */
    if (strncmp("\xff", "\x01", 1) == 1) r.n_ok++; else { printf("strncmp: unsigned gt failed\n"); r.n_fail++; }
    if (strncmp("\x01\xff", "\x01\x01", 2) == 1) r.n_ok++; else { printf("strncmp: unsigned second byte failed\n"); r.n_fail++; }

    /* random: derive b from a by mutation so the first differing position is
     * known by construction */
    srand(1717);
    for (int t = 0; t < 500; ++t) {
        char a[64], b[64];
        size_t la = (size_t)rand() % 40;
        rand_str_excl(a, la, 26, 0);
        size_t n = (size_t)rand() % 45;
        /* decide a difference position: either none, or dp < la (flip a char) */
        size_t dp = (size_t)rand() % (la + 1);  /* dp == la means "no difference" */
        for (size_t i = 0; i < la; ++i) {
            char c = a[i];
            if (i == dp) {
                c = (char)('A' + rand() % 26);   /* always differs from lowercase */
            }
            b[i] = c;
        }
        b[la] = 0;
        /* la == lb; a char was flipped at dp only when dp < la. Expected is:
         * 0 if no flip (dp == la) or dp >= n, else sign of a[dp] vs b[dp] */
        int expect = 0;
        if (dp < la && dp < n) {
            expect = (a[dp] > b[dp]) ? 1 : -1;
        }
        int got = strncmp(a, b, n);
        if (got == expect) r.n_ok++;
        else { printf("strncmp: random %d failed (la=%zu n=%zu dp=%zu)\n", t, la, n, dp); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strchr(void) {
    test_result_t r = {0, 0};

    /* fixed cases with hand-computed offsets */
    static const struct { const char *s; int c; long pos; } cases[] = {
        {"", 'a', -1}, {"", 0, 0},
        {"a", 'a', 0}, {"a", 'b', -1},
        {"hello", 'h', 0}, {"hello", 'e', 1}, {"hello", 'o', 4}, {"hello", 'x', -1},
        {"hello", 0, 5},            /* c=0: returns terminator */
        {"aaa", 'a', 0},
        {"abcdef", 'f', 5},
        {"abcabc", 'c', 2},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        char *got = strchr(cases[k].s, cases[k].c);
        int ok = (cases[k].pos < 0) ? (got == NULL) : (got == cases[k].s + cases[k].pos);
        if (ok) r.n_ok++;
        else { printf("strchr: \"%s\" c=%d -> %p, expected offset %ld\n",
                      cases[k].s, cases[k].c, (void*)got, cases[k].pos); r.n_fail++; }
    }

    /* c is compared as char: value 0x100+'a' truncates to 'a' */
    if (strchr("abc", 0x100 + 'b') == strchr("abc", 'b')) r.n_ok++;
    else { printf("strchr: truncation case failed\n"); r.n_fail++; }

    /* random planted cases: fill with chars guaranteed != c, then either
     * plant c at position p (expected: s+p) or don't (expected: NULL) */
    srand(101);
    for (int t = 0; t < 500; ++t) {
        char s[64];
        size_t len = (size_t)rand() % 50;
        char c = (char)('a' + rand() % 7);   /* alphabet is 5, so 'f'/'g' absent */
        rand_str_excl(s, len, 5, c);
        int planted = (len > 0) && (rand() % 2);
        size_t p = planted ? (size_t)rand() % len : 0;
        if (planted) s[p] = c;
        char *got = strchr(s, c);
        int ok = planted ? (got == s + p) : (got == NULL);
        if (ok) r.n_ok++; else { printf("strchr: random %d failed (s=\"%s\" c=%c planted=%d)\n", t, s, c, planted); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strrchr(void) {
    test_result_t r = {0, 0};

    /* fixed cases with hand-computed offsets */
    static const struct { const char *s; int c; long pos; } cases[] = {
        {"", 'a', -1}, {"", 0, 0},
        {"a", 'a', 0}, {"a", 'b', -1},
        {"hello", 'h', 0}, {"hello", 'l', 3}, {"hello", 'o', 4}, {"hello", 'x', -1},
        {"hello", 0, 5},            /* c=0: returns terminator */
        {"aaa", 'a', 2},
        {"abcdef", 'f', 5},
        {"abcabc", 'c', 5},
        {"abcabc", 'a', 3},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        char *got = strrchr(cases[k].s, cases[k].c);
        int ok = (cases[k].pos < 0) ? (got == NULL) : (got == cases[k].s + cases[k].pos);
        if (ok) r.n_ok++;
        else { printf("strrchr: \"%s\" c=%d -> %p, expected offset %ld\n",
                      cases[k].s, cases[k].c, (void*)got, cases[k].pos); r.n_fail++; }
    }

    if (strrchr("abc", 0x100 + 'b') == strrchr("abc", 'b')) r.n_ok++;
    else { printf("strrchr: truncation case failed\n"); r.n_fail++; }

    /* random planted cases: fill with chars guaranteed != c, then either
     * plant c at position p (it is the only occurrence, so the last) or
     * don't (expected: NULL) */
    srand(303);
    for (int t = 0; t < 500; ++t) {
        char s[64];
        size_t len = (size_t)rand() % 50;
        char c = (char)('a' + rand() % 7);   /* alphabet is 5, so 'f'/'g' absent */
        rand_str_excl(s, len, 5, c);
        int planted = (len > 0) && (rand() % 2);
        size_t p = planted ? (size_t)rand() % len : 0;
        if (planted) s[p] = c;
        char *got = strrchr(s, c);
        int ok = planted ? (got == s + p) : (got == NULL);
        if (ok) r.n_ok++; else { printf("strrchr: random %d failed (s=\"%s\" c=%c planted=%d)\n", t, s, c, planted); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strstr(void) {
    test_result_t r = {0, 0};

    /* fixed cases with hand-computed offsets */
    static const struct { const char *h, *n; long pos; } cases[] = {
        {"", "", 0}, {"", "a", -1}, {"a", "", 0}, {"abc", "", 0},
        {"abc", "a", 0}, {"abc", "b", 1}, {"abc", "c", 2}, {"abc", "abc", 0},
        {"abc", "abcd", -1}, {"abc", "d", -1},
        {"hello world", "hello", 0}, {"hello world", "world", 6},
        {"hello world", "lo wo", 3}, {"hello world", "lo wox", -1},
        {"aaab", "aab", 1},       /* partial match then restart */
        {"ababab", "abab", 0},    /* overlapping candidates */
        {"mississippi", "issip", 4},
        {"a", "aa", -1},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        char *got = strstr(cases[k].h, cases[k].n);
        int ok = (cases[k].pos < 0) ? (got == NULL) : (got == cases[k].h + cases[k].pos);
        if (ok) r.n_ok++;
        else { printf("strstr: \"%s\" in \"%s\" -> %p, expected offset %ld\n",
                      cases[k].n, cases[k].h, (void*)got, cases[k].pos); r.n_fail++; }
    }

    /* random planted cases: plant needle at position p in the haystack, then
     * only verify that the returned pointer references a genuine occurrence
     * (a fixed-length comparison), not a specific position */
    srand(40404);
    for (int t = 0; t < 500; ++t) {
        char h[80], n[12];
        size_t ln = 1 + (size_t)rand() % 6;
        rand_str_excl(n, ln, 3, 0);
        size_t lh = ln + (size_t)rand() % 40;
        size_t p = (size_t)rand() % (lh - ln + 1);
        rand_str_excl(h, lh, 3, 0);
        for (size_t j = 0; j < ln; ++j) h[p + j] = n[j];
        char *got = strstr(h, n);
        /* must find some real match: got in [h, h+lh-ln] and matches there */
        int ok = got != NULL && got >= h && got <= h + (lh - ln);
        for (size_t j = 0; ok && j < ln; ++j) {
            if (got[j] != n[j]) ok = 0;
        }
        if (ok) r.n_ok++; else { printf("strstr: random planted %d failed (n=\"%s\" h=\"%s\")\n", t, n, h); r.n_fail++; }
    }

    /* random absent cases: haystack built from chars disjoint from needle[0],
     * so no match can exist by construction */
    for (int t = 0; t < 300; ++t) {
        char h[80], n[12];
        size_t ln = 1 + (size_t)rand() % 6;
        rand_str_excl(n, ln, 3, 0);     /* needle first char in 'a'..'c' */
        size_t lh = (size_t)rand() % 60;
        rand_str_excl(h, lh, 3, 0);
        for (size_t i = 0; i < lh; ++i) h[i] = (char)('d' + rand() % 3);  /* 'd'..'f' */
        h[lh] = 0;
        if (strstr(h, n) == NULL) r.n_ok++;
        else { printf("strstr: random absent %d failed (n=\"%s\" h=\"%s\")\n", t, n, h); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strspn(void) {
    test_result_t r = {0, 0};

    /* fixed cases with hand-computed answers */
    static const struct { const char *s, *accept; size_t expect; } cases[] = {
        {"", "", 0}, {"", "abc", 0}, {"abc", "", 0},
        {"a", "a", 1}, {"a", "b", 0}, {"aaa", "a", 3},
        {"abc", "abc", 3}, {"abc", "cba", 3},  /* order of accept doesn't matter */
        {"abcabc", "ab", 2},                   /* stops at 'c' */
        {"abcd", "ab", 2}, {"dcba", "ab", 0},
        {"hello", "helo", 5}, {"hello", "xyz", 0},
        {"123abc", "0123456789", 3},
        {"  trim", " ", 2}, {"trailing   ", " ", 0}, {"   ", " ", 3},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        size_t got = strspn(cases[k].s, cases[k].accept);
        if (got == cases[k].expect) r.n_ok++;
        else { printf("strspn: \"%s\" accept \"%s\" -> %zu, expected %zu\n",
                      cases[k].s, cases[k].accept, got, cases[k].expect); r.n_fail++; }
    }

    /* random planted cases: s = k chars drawn from accept, then either NUL
     * or a char outside the alphabet (guaranteed not in accept).
     * Expected span = k by construction. */
    srand(505);
    for (int t = 0; t < 500; ++t) {
        char s[64], accept[8];
        size_t la = (size_t)rand() % 5;
        for (size_t i = 0; i < la; ++i) accept[i] = (char)('a' + rand() % 4);
        accept[la] = 0;
        size_t k;
        if (la == 0) {
            /* empty accept: span must be 0 regardless of s */
            k = 0;
        } else {
            k = (size_t)rand() % 20;
        }
        for (size_t i = 0; i < k; ++i) s[i] = accept[(size_t)rand() % la];
        if (rand() % 2) {
            s[k] = 'e';             /* outside 'a'..'d': guaranteed stop */
            s[k + 1] = 0;
        } else {
            s[k] = 0;
        }
        size_t got = strspn(s, accept);
        if (got == k) r.n_ok++; else { printf("strspn: random %d failed (s=\"%s\" k=%zu got=%zu)\n", t, s, k, got); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strcspn(void) {
    test_result_t r = {0, 0};

    /* fixed cases with hand-computed answers */
    static const struct { const char *s, *reject; size_t expect; } cases[] = {
        {"", "", 0}, {"", "abc", 0}, {"abc", "", 3},  /* empty reject: full length */
        {"a", "a", 0}, {"a", "b", 1}, {"aaa", "a", 0},
        {"abc", "abc", 0}, {"abc", "cba", 0},
        {"abcabc", "ab", 0},
        {"abcd", "ab", 0}, {"dcba", "ab", 2},
        {"hello", "helo", 0}, {"hello", "xyz", 5},
        {"123abc", "0123456789", 0},
        {"  trim", " ", 0}, {"trailing   ", " ", 8},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        size_t got = strcspn(cases[k].s, cases[k].reject);
        if (got == cases[k].expect) r.n_ok++;
        else { printf("strcspn: \"%s\" reject \"%s\" -> %zu, expected %zu\n",
                      cases[k].s, cases[k].reject, got, cases[k].expect); r.n_fail++; }
    }

    /* random planted cases: s = k chars each guaranteed not in reject, then
     * either NUL or a char drawn from reject (guaranteed stop).
     * Expected span = k by construction. */
    srand(909);
    for (int t = 0; t < 500; ++t) {
        char s[64], reject[8];
        size_t lr = (size_t)rand() % 4;          /* reject covers at most 3 of 6 chars */
        for (size_t i = 0; i < lr; ++i) reject[i] = (char)('a' + rand() % 6);
        reject[lr] = 0;
        size_t k = (size_t)rand() % 20;
        for (size_t i = 0; i < k; ++i) {
            /* pick a char not in reject: scan alphabet from a random start */
            size_t start = (size_t)rand() % 6;
            char c = 'a';
            for (size_t d = 0; d < 6; ++d) {
                c = (char)('a' + (start + d) % 6);
                if (!in_set(c, reject)) break;
            }
            s[i] = c;
        }
        if (lr > 0 && (rand() % 2)) {
            s[k] = reject[(size_t)rand() % lr];  /* guaranteed stop char */
            s[k + 1] = 0;
        } else {
            s[k] = 0;
        }
        size_t got = strcspn(s, reject);
        if (got == k) r.n_ok++; else { printf("strcspn: random %d failed (s=\"%s\" k=%zu got=%zu)\n", t, s, k, got); r.n_fail++; }
    }
    return r;
}

static test_result_t test_strpbrk(void) {
    test_result_t r = {0, 0};

    /* fixed cases with hand-computed offsets */
    static const struct { const char *s, *accept; long pos; } cases[] = {
        {"", "", -1}, {"", "abc", -1}, {"abc", "", -1},
        {"a", "a", 0}, {"a", "b", -1}, {"aaa", "a", 0},
        {"abc", "abc", 0}, {"abc", "cba", 0},
        {"abcabc", "ab", 0}, {"abcd", "ab", 0}, {"dcba", "ab", 2},
        {"hello", "helo", 0}, {"hello", "xyz", -1},
        {"123abc", "0123456789", 0},
        {"  trim", " ", 0}, {"trailing   ", " ", 8},
    };
    for (size_t k = 0; k < sizeof(cases)/sizeof(cases[0]); ++k) {
        char *got = strpbrk(cases[k].s, cases[k].accept);
        int ok = (cases[k].pos < 0) ? (got == NULL) : (got == cases[k].s + cases[k].pos);
        if (ok) r.n_ok++;
        else { printf("strpbrk: \"%s\" accept \"%s\" -> %p, expected offset %ld\n",
                      cases[k].s, cases[k].accept, (void*)got, cases[k].pos); r.n_fail++; }
    }

    /* random planted cases: prefix of s built from chars guaranteed not in
     * accept, then either NUL (expected: NULL) or an accept char planted at
     * position k (guaranteed first hit, expected: s+k) */
    srand(111);
    for (int t = 0; t < 500; ++t) {
        char s[64], accept[8];
        size_t la = (size_t)rand() % 5;
        for (size_t i = 0; i < la; ++i) accept[i] = (char)('a' + rand() % 5);
        accept[la] = 0;
        size_t k = (size_t)rand() % 20;
        for (size_t i = 0; i < k; ++i) {
            /* pick a char not in accept: scan alphabet from a random start */
            size_t start = (size_t)rand() % 5;
            char c = 'a';
            for (size_t d = 0; d < 5; ++d) {
                c = (char)('a' + (start + d) % 5);
                if (!in_set(c, accept)) break;
            }
            s[i] = c;
        }
        int planted = (la > 0) && (rand() % 2);
        if (planted) {
            s[k] = accept[(size_t)rand() % la];
            s[k + 1] = 0;
        } else {
            s[k] = 0;
        }
        char *got = strpbrk(s, accept);
        int ok = planted ? (got == s + k) : (got == NULL);
        if (ok) r.n_ok++; else { printf("strpbrk: random %d failed (s=\"%s\" planted=%d)\n", t, s, planted); r.n_fail++; }
    }
    return r;
}

test_def_t test_list[] = {
    {"strlen", test_strlen},
    {"strcpy", test_strcpy},
    {"strncpy", test_strncpy},
    {"strcat", test_strcat},
    {"strncat", test_strncat},
    {"strcmp", test_strcmp},
    {"strncmp", test_strncmp},
    {"strchr", test_strchr},
    {"strrchr", test_strrchr},
    {"strstr", test_strstr},
    {"strspn", test_strspn},
    {"strcspn", test_strcspn},
    {"strpbrk", test_strpbrk},
    {NULL, NULL},
};
