#include <cstdio>
struct X { unsigned char p[4]; long s; long t; void F() { t = s; } };
int main()
{
    X x; x.s = 4; x.t = 0; x.F(); if (x.t != 4) { std::printf("007ffd50_TEST FAIL\n"); return 1; }

    std::printf("007ffd50_TEST PASS\n");
    return 0;
}