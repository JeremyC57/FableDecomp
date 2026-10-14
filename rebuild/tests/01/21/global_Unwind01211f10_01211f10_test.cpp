#include <cstdio>
static int g_r = 0; struct I { virtual void R(long) { ++g_r; } }; struct B { virtual void v() {} ~B() {} }; struct X : B { I* p; ~X() { if (p) { p->R(1); p = 0; } } };
static bool Check() { I i; { X x; x.p = &i; } { X y; y.p = 0; } return g_r == 1; }

int main()
{
    if (!Check()) { std::printf("00c17b10_TEST FAIL\n"); return 1; }
    std::printf("00c17b10_TEST PASS\n");
    return 0;
}