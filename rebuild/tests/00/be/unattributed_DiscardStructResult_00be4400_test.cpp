#include <cstdio>
static int g_c = 0; struct R { long v[6]; }; struct D { R M() { ++g_c; R r; r.v[0] = 1; return r; } void F() { M(); } };
int main()
{
    D d; d.F(); if (g_c != 1) { std::printf("00be4400_TEST FAIL\n"); return 1; }

    std::printf("00be4400_TEST PASS\n");
    return 0;
}