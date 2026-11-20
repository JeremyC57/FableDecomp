#include <cstdio>
struct X { unsigned char p[4]; long c; unsigned char e; void F() { if (e) ++c; } };
int main()
{
    X x; x.c = 0; x.e = 0; x.F(); x.e = 1; x.F(); if (x.c != 1) { std::printf("007aed30_TEST FAIL\n"); return 1; }

    std::printf("007aed30_TEST PASS\n");
    return 0;
}