#include <cstdio>
static long g_v = -1; struct X { void M(long v) { g_v = v; } void F() { M(1); } };
int main()
{
    X x; x.F(); if (g_v != 1) { std::printf("008f5cf0_TEST FAIL\n"); return 1; }

    std::printf("008f5cf0_TEST PASS\n");
    return 0;
}