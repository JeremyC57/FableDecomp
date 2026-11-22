#include <cstdio>
static long g_v = -1; struct X { void M(long v) { g_v = v; } void F() { M(2); } };
int main()
{
    X x; x.F(); if (g_v != 2) { std::printf("008fa300_TEST FAIL\n"); return 1; }

    std::printf("008fa300_TEST PASS\n");
    return 0;
}