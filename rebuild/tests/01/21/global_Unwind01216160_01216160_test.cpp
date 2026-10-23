#include <cstdio>
static int g_t = 0, g_b = 0; struct Tk { virtual ~Tk() { ++g_t; } }; struct B { virtual void v() {} ~B() { ++g_b; } }; struct Sl { virtual void v() {} ~Sl() {} }; struct X : B { unsigned char p[0x1ec]; Sl s; unsigned char q[0x2c]; Tk* t; ~X() { if (t) delete t; } };
static bool Check() { { X* x = new X; x->t = new Tk; delete x; } { X* y = new X; y->t = 0; delete y; } return g_t == 1 && g_b == 2; }

int main()
{
    if (!Check()) { std::printf("00c55130_TEST FAIL\n"); return 1; }
    std::printf("01216160_TEST PASS\n");
    return 0;
}