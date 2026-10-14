#include <cstdio>
static long g_v = -1; struct X { void M(long v) { g_v = v; } void F() { M(19608120); } };
int main()
{
    X x; x.F(); if (g_v != 19608120) { std::printf("00c1c220_TEST FAIL\n"); return 1; }

    std::printf("00c1c220_TEST PASS\n");
    return 0;
}