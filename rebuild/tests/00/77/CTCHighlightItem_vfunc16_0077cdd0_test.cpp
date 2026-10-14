#include <cstdio>
static int g_n = 0; struct X { void U() { ++g_n; } void F(bool e) { if (e) U(); } };
int main()
{
    X x; x.F(false); x.F(true); if (g_n != 1) { std::printf("0077cdd0_TEST FAIL\n"); return 1; }

    std::printf("0077cdd0_TEST PASS\n");
    return 0;
}