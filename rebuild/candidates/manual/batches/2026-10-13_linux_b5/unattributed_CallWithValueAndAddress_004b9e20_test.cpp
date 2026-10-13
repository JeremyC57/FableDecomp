#include <cstdio>
static long g_v = 0; static long* g_s = 0; static void __stdcall H(long v, long* s) { g_v = v; g_s = s; *s = v + 1; }
struct O { void F(long v) { H(v, &v); } };
int main()
{
    O o; o.F(41); if (g_v != 41 || !g_s) { std::printf("004b9e20_TEST FAIL\n"); return 1; }

    std::printf("004b9e20_TEST PASS\n");
    return 0;
}
