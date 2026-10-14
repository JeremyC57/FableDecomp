#include <cstdio>
struct X { unsigned char p[4]; long a; long b; void F() { a = 0; b = 2; } };
int main()
{
    X x; x.F(); if (x.a != 0 || x.b != 2) { std::printf("0071ab10_TEST FAIL\n"); return 1; }

    std::printf("0071ab10_TEST PASS\n");
    return 0;
}