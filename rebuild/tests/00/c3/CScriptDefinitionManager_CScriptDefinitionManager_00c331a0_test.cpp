#include <cstdio>
static int g_s = 0, g_b = 0; struct B { virtual void v() {} ~B() { ++g_b; } }; struct X : B { void S() { ++g_s; } ~X() { S(); } };
int main()
{
    { X x; } if (g_s != 1 || g_b != 1) { std::printf("00c331a0_TEST FAIL\n"); return 1; }
    std::printf("00c331a0_TEST PASS\n");
    return 0;
}