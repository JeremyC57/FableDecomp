#include <cstdio>
static long g_k = -1; static void* g_v = 0; struct X { void M(void* v, long k) { g_v = v; g_k = k; } void F(void* v) { M(v, 8); } };
int main()
{
    X x; int i; x.F(&i); if (g_v != &i || g_k != 8) { std::printf("008300a0_TEST FAIL\n"); return 1; }

    std::printf("008300a0_TEST PASS\n");
    return 0;
}