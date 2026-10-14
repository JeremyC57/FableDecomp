#include <cstdio>
static long g_k = -1; static void* g_v = 0; struct X { void M(void* v, long k) { g_v = v; g_k = k; } void F(void* v) { M(v, 10); } };
int main()
{
    X x; int i; x.F(&i); if (g_v != &i || g_k != 10) { std::printf("008300c0_TEST FAIL\n"); return 1; }

    std::printf("008300c0_TEST PASS\n");
    return 0;
}