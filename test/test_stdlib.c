/* Note: this file is AI generated. */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "test.h"

static int cmp_int(const void *a, const void *b) {
    int x = *(const int*)a, y = *(const int*)b;
    return (x > y) - (x < y);
}

/* verify a is sorted and is a permutation of before (values in [-20, 19]) */
static void check_case(test_result_t *r, const int *a, size_t n, const int *before) {
    for (size_t i = 1; i < n; ++i) {
        if (a[i-1] > a[i]) {
            printf("qsort: not sorted at index %zu (%d > %d), n=%zu\n", i, a[i-1], a[i], n);
            r->n_fail++;
            return;
        }
    }
    int cnt_before[40] = {0}, cnt_after[40] = {0};
    for (size_t i = 0; i < n; ++i) cnt_before[before[i] + 20]++;
    for (size_t i = 0; i < n; ++i) cnt_after[a[i] + 20]++;
    for (int v = 0; v < 40; ++v) {
        if (cnt_before[v] != cnt_after[v]) {
            printf("qsort: value %d count changed (%d -> %d), n=%zu\n", v - 20, cnt_before[v], cnt_after[v], n);
            r->n_fail++;
            return;
        }
    }
    r->n_ok++;
}

static test_result_t test_qsort(void) {
    test_result_t r = {0, 0};
    int arr[512];

    /* fixed sizes, values in [-20, 19] to fit the counting table */
    static const size_t sizes[] = {0, 1, 2, 3, 4, 5, 10, 33, 100, 257, 512};
    srand(424242);
    for (size_t k = 0; k < sizeof(sizes)/sizeof(sizes[0]); ++k) {
        size_t n = sizes[k];
        int before[512];
        for (size_t i = 0; i < n; ++i) {
            arr[i] = (int)(rand() % 40) - 20;
            before[i] = arr[i];
        }
        qsort(arr, n, sizeof(int), cmp_int);
        check_case(&r, arr, n, before);
    }

    /* already sorted and reverse sorted */
    for (size_t i = 0; i < 512; ++i) arr[i] = (int)i;
    qsort(arr, 512, sizeof(int), cmp_int);
    {
        int ok = 1;
        for (size_t i = 0; i < 512; ++i) if (arr[i] != (int)i) ok = 0;
        if (ok) r.n_ok++; else { printf("qsort: sorted input failed\n"); r.n_fail++; }
    }
    for (size_t i = 0; i < 512; ++i) arr[i] = (int)(512 - i);
    qsort(arr, 512, sizeof(int), cmp_int);
    {
        int ok = 1;
        for (size_t i = 0; i < 512; ++i) if (arr[i] != (int)(i + 1)) ok = 0;
        if (ok) r.n_ok++; else { printf("qsort: reverse input failed\n"); r.n_fail++; }
    }

    /* all equal */
    for (size_t i = 0; i < 512; ++i) arr[i] = 7;
    qsort(arr, 512, sizeof(int), cmp_int);
    {
        int ok = 1;
        for (size_t i = 0; i < 512; ++i) if (arr[i] != 7) ok = 0;
        if (ok) r.n_ok++; else { printf("qsort: all-equal failed\n"); r.n_fail++; }
    }

    /* random trials with heavy duplicates, values in [-3, 2] */
    for (int t = 0; t < 200; ++t) {
        size_t n = (size_t)rand() % 200 + 1;
        int before[200];
        for (size_t i = 0; i < n; ++i) {
            arr[i] = (int)(rand() % 6) - 3;
            before[i] = arr[i];
        }
        qsort(arr, n, sizeof(int), cmp_int);
        int ok = 1;
        for (size_t i = 1; i < n; ++i) if (arr[i-1] > arr[i]) ok = 0;
        int cb[6] = {0}, ca[6] = {0};
        for (size_t i = 0; i < n; ++i) cb[before[i] + 3]++;
        for (size_t i = 0; i < n; ++i) ca[arr[i] + 3]++;
        for (int v = 0; v < 6; ++v) if (cb[v] != ca[v]) ok = 0;
        if (ok) r.n_ok++; else { printf("qsort: dup trial %d failed\n", t); r.n_fail++; }
    }
    return r;
}

/* build a random non-decreasing array; returns n; lo/hi get value range */
static size_t make_sorted(int *arr, size_t cap, int *lo, int *hi) {
    size_t n = 1 + (size_t)rand() % cap;
    int prev = -50;
    for (size_t i = 0; i < n; ++i) {
        prev += (int)(rand() % 4);   /* non-decreasing, many dups */
        arr[i] = prev;
    }
    *lo = arr[0];
    *hi = arr[n - 1];
    return n;
}

static int member_of(const int *arr, size_t n, int key) {
    for (size_t i = 0; i < n; ++i) if (arr[i] == key) return 1;
    return 0;
}

static test_result_t test_bsearch(void) {
    test_result_t r = {0, 0};
    int arr[256];
    int key;

    /* empty array: nothing can be found */
    key = 5;
    if (bsearch(&key, arr, 0, sizeof(int), cmp_int) == NULL) r.n_ok++;
    else { printf("bsearch: empty array found something\n"); r.n_fail++; }

    /* single element */
    arr[0] = 42;
    key = 42;
    if (bsearch(&key, arr, 1, sizeof(int), cmp_int) == &arr[0]) r.n_ok++;
    else { printf("bsearch: single present failed\n"); r.n_fail++; }
    key = 41;
    if (bsearch(&key, arr, 1, sizeof(int), cmp_int) == NULL) r.n_ok++;
    else { printf("bsearch: single absent failed\n"); r.n_fail++; }

    /* fixed small array: every position and gap between elements */
    for (int i = 0; i < 10; ++i) arr[i] = i * 2;  /* 0,2,4,...,18 */
    for (int v = -1; v <= 19; ++v) {
        key = v;
        void *got = bsearch(&key, arr, 10, sizeof(int), cmp_int);
        if (v >= 0 && v % 2 == 0) {
            /* present; result must point to an element equal to key */
            if (got != NULL && *(const int*)got == v) r.n_ok++;
            else { printf("bsearch: v=%d present but got %p\n", v, got); r.n_fail++; }
        } else {
            if (got == NULL) r.n_ok++;
            else { printf("bsearch: v=%d absent but got %p\n", v, got); r.n_fail++; }
        }
    }

    /* duplicates: bsearch may return any matching element */
    for (int i = 0; i < 10; ++i) arr[i] = 5;
    key = 5;
    {
        void *got = bsearch(&key, arr, 10, sizeof(int), cmp_int);
        if (got != NULL && *(const int*)got == 5) r.n_ok++;
        else { printf("bsearch: duplicates failed\n"); r.n_fail++; }
    }

    /* negative keys */
    for (int i = 0; i < 10; ++i) arr[i] = -100 + i * 10;
    key = -70;
    {
        void *got = bsearch(&key, arr, 10, sizeof(int), cmp_int);
        if (got != NULL && *(const int*)got == -70) r.n_ok++;
        else { printf("bsearch: negative key failed\n"); r.n_fail++; }
    }
    key = -71;
    if (bsearch(&key, arr, 10, sizeof(int), cmp_int) == NULL) r.n_ok++;
    else { printf("bsearch: negative absent failed\n"); r.n_fail++; }

    /* random sorted arrays with duplicates; probe present and absent keys */
    srand(31415);
    for (int t = 0; t < 300; ++t) {
        int lo, hi;
        size_t n = make_sorted(arr, 100, &lo, &hi);
        if (rand() % 2) {
            /* arbitrary key in range: found iff it is a member */
            key = lo + (int)(rand() % (size_t)(hi - lo + 1));
            void *got = bsearch(&key, arr, n, sizeof(int), cmp_int);
            if (member_of(arr, n, key)) {
                if (got != NULL && *(const int*)got == key) r.n_ok++;
                else { printf("bsearch: trial %d key=%d present but not found\n", t, key); r.n_fail++; }
            } else {
                if (got == NULL) r.n_ok++;
                else { printf("bsearch: trial %d key=%d absent but found\n", t, key); r.n_fail++; }
            }
        } else {
            /* an actual array element: must be found */
            key = arr[(size_t)rand() % n];
            void *got = bsearch(&key, arr, n, sizeof(int), cmp_int);
            if (got != NULL && *(const int*)got == key) r.n_ok++;
            else { printf("bsearch: trial %d member key=%d not found\n", t, key); r.n_fail++; }
        }
    }
    return r;
}

test_def_t test_list[] = {
    {"qsort", test_qsort},
    {"bsearch", test_bsearch},
    {NULL, NULL},
};
