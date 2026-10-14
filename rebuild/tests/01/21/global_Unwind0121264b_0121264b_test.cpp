#include <cstdio>
static int g_a = 0; struct E; struct Rg { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void i() {} virtual void A(E*) { ++g_a; } }; struct O { unsigned char p[0x74]; Rg r; }; struct Lnk { void* a; void* b; void* c; Lnk() : a(0), b(0), c(0) {} ~Lnk() {} }; struct E { O* o; void* d; Lnk l; Lnk m; E(O* x, void* y) : o(x), d(y), l(), m() { o->r.A(this); } };
static bool Check() { O o; int i; E e(&o, &i); return g_a == 1 && e.o == &o && e.d == &i && e.l.a == 0 && e.m.c == 0; }

int main()
{
    if (!Check()) { std::printf("00c1e6d0_TEST FAIL\n"); return 1; }
    std::printf("00c1e6d0_TEST PASS\n");
    return 0;
}