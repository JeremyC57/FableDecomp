#include <cstdio>
static int g_n = 0; struct B { virtual void v() {} ~B() { ++g_n; } }; struct X : B { ~X() {} };
int main()
{
    { X x; } if (g_n != 1) { std::printf("00c697c0_TEST FAIL\n"); return 1; }

    std::printf("00c697c0_TEST PASS\n");
    return 0;
}