#include <cstdio>
static long g_k = 0; struct X { unsigned char p[4]; unsigned char a; unsigned char q[3]; unsigned char b; void B(long k) { g_k = k; } void F() { a = 1; b = 0; B(6); } };
int main()
{
    X x; x.F(); if (g_k != 6 || x.a != 1 || x.b != 0) { std::printf("007d29f0_TEST FAIL\n"); return 1; }

    std::printf("007d29f0_TEST PASS\n");
    return 0;
}