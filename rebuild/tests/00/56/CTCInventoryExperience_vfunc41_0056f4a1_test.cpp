#include <cstdio>
static int g_a = 0, g_b = 0; static void Fin() { ++g_b; } struct X { void P() { ++g_a; } void F() { P(); Fin(); } };
int main()
{
    X x; x.F(); if (g_a != 1 || g_b != 1) { std::printf("0056f4a1_TEST FAIL\n"); return 1; }

    std::printf("0056f4a1_TEST PASS\n");
    return 0;
}