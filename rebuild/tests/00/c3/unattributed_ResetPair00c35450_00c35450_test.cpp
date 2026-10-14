#include <cstdio>
struct X { unsigned char p[4]; long a; long b; void F() { a = 3; b = 0; } };
int main()
{
    X x; x.F(); if (x.a != 3 || x.b != 0) { std::printf("00c35450_TEST FAIL\n"); return 1; }

    std::printf("00c35450_TEST PASS\n");
    return 0;
}