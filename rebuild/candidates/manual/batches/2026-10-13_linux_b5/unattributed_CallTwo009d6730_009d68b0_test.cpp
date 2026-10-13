#include <cstdio>
static int g_s = 0; struct T { void A() { g_s = g_s * 10 + 1; } void B() { g_s = g_s * 10 + 2; } void F() { A(); B(); } };
int main()
{
    T t; t.F(); if (g_s != 12) { std::printf("009d68b0_TEST FAIL\n"); return 1; }

    std::printf("009d68b0_TEST PASS\n");
    return 0;
}
