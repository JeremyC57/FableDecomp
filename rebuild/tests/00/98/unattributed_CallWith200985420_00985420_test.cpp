#include <cstdio>
static long g_a = -1, g_b = -1; struct X { void M(long a, long b) { g_a = a; g_b = b; } void F() { M(1073741824, 1); } };
int main()
{
    X x; x.F(); if (g_a != 1073741824 || g_b != 1) { std::printf("00985420_TEST FAIL\n"); return 1; }

    std::printf("00985420_TEST PASS\n");
    return 0;
}