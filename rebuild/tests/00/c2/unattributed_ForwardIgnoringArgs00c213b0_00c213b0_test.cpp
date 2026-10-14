#include <cstdio>
static int g_n = 0; struct X { void M() { ++g_n; } void F(long a0) { M(); } };
int main()
{
    X x; x.F(0); if (g_n != 1) { std::printf("00c213b0_TEST FAIL\n"); return 1; }

    std::printf("00c213b0_TEST PASS\n");
    return 0;
}