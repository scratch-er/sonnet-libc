#include <stdio.h>

int putchar(int c) {
    char ch = c;
    putch(c);
    return ch;
}

int puts(const char *str) {
    int cnt;
    for (cnt=0; str[cnt]; ++cnt) {
        putch(str[cnt]);
    }
    putch('\n');
    return cnt;
}
