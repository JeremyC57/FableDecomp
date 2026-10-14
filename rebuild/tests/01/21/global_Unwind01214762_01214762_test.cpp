#include <cstdio>
static int g_a = 0, g_b = 0; struct C; struct Rg { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void R(C*) { ++g_a; } }; struct O { unsigned char p[0x40]; Rg r; }; struct P { unsigned char p[4]; O* o; Rg u; }; struct B { virtual void v() {} ~B() {} }; struct Sl { virtual void v() {} ~Sl() {} }; struct C : B { P* p; unsigned char q[0x38]; Sl s; ~C() { p->o->r.R(this); p->u.R(this); } };
static bool Check() { O o; P p; p.o = &o; { C c; c.p = &p; } return g_a == 2; }

int main()
{
    if (!Check()) { std::printf("00c38470_TEST FAIL\n"); return 1; }
    std::printf("00c38470_TEST PASS\n");
    return 0;
}