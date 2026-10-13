#include <cstdio>
static long g_m = 0; static int g_c = 0; struct L { void M() { ++g_c; } }; static L g_l;
static L* __stdcall Lookup(long m, long) { g_m = m; return &g_l; }
struct A { void P() { Lookup(16, 0)->M(); } };
int main()
{
    A a; a.P(); if (g_m != 16 || g_c != 1) { std::printf("008424d0_TEST FAIL\n"); return 1; }

    std::printf("008424d0_TEST PASS\n");
    return 0;
}
