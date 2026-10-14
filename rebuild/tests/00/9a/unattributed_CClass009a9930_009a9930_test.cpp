#include <cstdio>
static int g_n = 0; struct A { }; struct X { unsigned char b[4]; void I() { ++g_n; } X(const A&) { I(); } };
int main()
{
    A al; X x(al); if (g_n != 1) { std::printf("009a9930_TEST FAIL\n"); return 1; }

    std::printf("009a9930_TEST PASS\n");
    return 0;
}