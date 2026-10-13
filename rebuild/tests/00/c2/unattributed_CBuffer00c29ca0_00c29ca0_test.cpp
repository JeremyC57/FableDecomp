#include <cstdio>
struct X { unsigned char p[4]; unsigned long* d; unsigned long n, c; ~X() { delete d; d = 0; n = 0; c = 0; } };
int main()
{
    X x; x.d = new unsigned long; x.n = 3; x.c = 4; x.~X(); if (x.d || x.n || x.c) { std::printf("00c29ca0_TEST FAIL\n"); return 1; }

    std::printf("00c29ca0_TEST PASS\n");
    return 0;
}