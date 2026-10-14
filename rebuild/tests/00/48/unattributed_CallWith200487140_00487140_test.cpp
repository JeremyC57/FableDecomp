#include <cstdio>
static long g_a = -1, g_b = -1; struct X { void M(long a, long b) { g_a = a; g_b = b; } void F() { M(17, 0); } };
int main()
{
    X x; x.F(); if (g_a != 17 || g_b != 0) { std::printf("00487140_TEST FAIL\n"); return 1; }

    std::printf("00487140_TEST PASS\n");
    return 0;
}