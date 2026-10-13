#include <cstdio>
static int g_d = 0; struct K { virtual void v() {} virtual void D() { g_d += 100; } long r; };
struct P { K* b; ~P() { if (b) { if (--b->r == 0) b->D(); b = 0; } } };
struct B { long x; ~B() { ++g_d; } }; struct X : B { P p; ~X() {} };
int main()
{
    K k; k.r = 1; { X x; x.p.b = &k; } if (g_d != 101) { std::printf("005e0ab0_TEST FAIL\n"); return 1; }

    std::printf("005e0ab0_TEST PASS\n");
    return 0;
}
