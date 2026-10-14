#include <cstdio>
static int g_d = 0; struct O { ~O() { ++g_d; } }; struct B { virtual void v() {} ~B() {} }; struct X : B { unsigned char p[4]; O* o; ~X() { delete o; o = 0; } };
static bool Check() { { X x; x.o = new O; } { X y; y.o = 0; } return g_d == 1; }

int main()
{
    if (!Check()) { std::printf("00c16130_TEST FAIL\n"); return 1; }
    std::printf("00c16130_TEST PASS\n");
    return 0;
}