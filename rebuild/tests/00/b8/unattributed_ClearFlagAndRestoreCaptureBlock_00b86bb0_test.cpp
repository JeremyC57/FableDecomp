#include <cstdio>
static int g_r = 0; struct M { void Restore() { ++g_r; } }; static M g_m; static M* g_p = &g_m;
struct U { unsigned char pad[0x144]; bool c; void F() { c = false; g_p->Restore(); } };
int main()
{
    U u; u.c = true; u.F(); if (u.c || g_r != 1) { std::printf("00b86bb0_TEST FAIL\n"); return 1; }

    std::printf("00b86bb0_TEST PASS\n");
    return 0;
}