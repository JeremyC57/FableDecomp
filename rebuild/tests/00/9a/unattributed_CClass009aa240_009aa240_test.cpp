#include <cstdio>
static int g_n = 0; struct X { unsigned char b[4]; void I() { ++g_n; } X() { I(); } };
int main()
{
    X x; if (g_n != 1) { std::printf("009aa240_TEST FAIL\n"); return 1; }

    std::printf("009aa240_TEST PASS\n");
    return 0;
}