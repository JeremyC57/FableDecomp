#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static long g_r = 0; struct D { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void i() {} virtual void j() {} virtual void k() {} virtual void l() {} virtual void m() {} virtual void n() {} virtual void o() {} virtual void p() {} virtual void q() {} virtual void R(long k) { g_r = k; } }; struct X { unsigned char p[0x64]; D* d; void F() { L l(&g_g); if (d) { d->R(1); d = 0; } } };
static bool Check() { D d; X x; x.d = &d; x.F(); x.F(); return g_r == 1 && x.d == 0 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c088d0_TEST FAIL\n"); return 1; }
    std::printf("00c088d0_TEST PASS\n");
    return 0;
}