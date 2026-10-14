#include <cstdio>
static int g_r = 0, g_b = 0; struct C; struct Rg { virtual void R(C*) { ++g_r; } }; struct O { unsigned char p[0x38]; Rg r; }; struct B { virtual void v() {} ~B() { ++g_b; } }; struct C : B { O* o; ~C() { o->r.R(this); } };
static bool Check() { O o; { C c; c.o = &o; } return g_r == 1 && g_b == 1; }

int main()
{
    if (!Check()) { std::printf("00c1e180_TEST FAIL\n"); return 1; }
    std::printf("00c1e180_TEST PASS\n");
    return 0;
}