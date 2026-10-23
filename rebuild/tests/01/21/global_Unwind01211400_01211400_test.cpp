#include <cstdio>
static int g_b = 0, g_m = 0; struct B { virtual void v() {} unsigned char b[8]; B() { ++g_b; } ~B() {} }; struct M { unsigned char b[4]; M(void*) { ++g_m; } ~M() {} }; struct X : B { M m; X(void* s) : m(s) {} };
static bool Check() { X x(0); return g_b == 1 && g_m == 1; }

int main()
{
    if (!Check()) { std::printf("00c108e0_TEST FAIL\n"); return 1; }
    std::printf("01211400_TEST PASS\n");
    return 0;
}