#include <cstdio>
static int g_d = 0; struct P { virtual ~P() { ++g_d; } }; struct B { virtual void v() {} ~B() {} }; struct X : B { unsigned char q[0xe0]; P* p; ~X() { if (p) { delete p; p = 0; } } };
static bool Check() { { X x; x.p = new P; } { X y; y.p = 0; } return g_d == 1; }

int main()
{
    if (!Check()) { std::printf("00c4f220_TEST FAIL\n"); return 1; }
    std::printf("00c4f220_TEST PASS\n");
    return 0;
}