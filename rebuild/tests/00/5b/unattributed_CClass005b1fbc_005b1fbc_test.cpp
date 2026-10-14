#include <cstdio>
static int g_n = 0; struct B { unsigned char b[4]; B() { ++g_n; } }; struct X : B { long v; X() : v(0) {} };
int main()
{
    X x; if (g_n != 1 || x.v != 0) { std::printf("005b1fbc_TEST FAIL\n"); return 1; }

    std::printf("005b1fbc_TEST PASS\n");
    return 0;
}