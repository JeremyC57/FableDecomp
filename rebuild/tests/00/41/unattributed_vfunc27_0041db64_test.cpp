#include <cstdio>
static long g_v = 0; struct M { void S(const long& v) { g_v = v; } }; struct X { unsigned char p[8]; M m; void F(long v) { m.S(v); } };
int main()
{
    X x; x.F(5); if (g_v != 5) { std::printf("0041db64_TEST FAIL\n"); return 1; }

    std::printf("0041db64_TEST PASS\n");
    return 0;
}