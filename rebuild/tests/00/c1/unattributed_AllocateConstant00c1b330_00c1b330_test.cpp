#include <cstdio>
static unsigned int g_k = 0; static void* __cdecl C(unsigned int k) { g_k = k; return &g_k; } struct X { void* F() { return C(24); } };
int main()
{
    X x; if (x.F() != &g_k || g_k != 24) { std::printf("00c1b330_TEST FAIL\n"); return 1; }

    std::printf("00c1b330_TEST PASS\n");
    return 0;
}