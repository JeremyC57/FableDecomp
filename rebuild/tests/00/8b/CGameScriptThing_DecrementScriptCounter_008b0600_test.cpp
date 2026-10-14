#include <cstdio>
struct X { unsigned char p[4]; long c; void F() { ++c; } };
int main()
{
    X x; x.c = 1; x.F(); if (x.c != 2) { std::printf("008b0600_TEST FAIL\n"); return 1; }

    std::printf("008b0600_TEST PASS\n");
    return 0;
}