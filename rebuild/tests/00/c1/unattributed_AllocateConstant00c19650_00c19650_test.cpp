#include <cstdio>
static unsigned int g_k = 0; static void* __cdecl C(unsigned int k) { g_k = k; return &g_k; } struct X { void* F() { return C(20); } };
int main()
{
    X x; if (x.F() != &g_k || g_k != 20) { std::printf("00c19650_TEST FAIL\n"); return 1; }

    std::printf("00c19650_TEST PASS\n");
    return 0;
}