#include <cstdio>
static void* g_b = 0; static void __cdecl R(void* b) { g_b = b; } struct X { unsigned char p[4]; void* k; void F() { R(k); } };
int main()
{
    X x; int i; x.k = &i; x.F(); if (g_b != &i) { std::printf("00c463a0_TEST FAIL\n"); return 1; }

    std::printf("00c463a0_TEST PASS\n");
    return 0;
}