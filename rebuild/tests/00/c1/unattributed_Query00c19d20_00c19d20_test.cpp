#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct N { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void i() {} virtual void j() {} virtual void k() {} virtual void l() {} virtual void m() {} virtual void n() {} virtual void o() {} virtual long Q() { return 8; } }; struct X { N n; N* F(void*) { return &n; } long Q(void* k) { L l(&g_g); return F(k)->Q(); } };
static bool Check() { X x; return x.Q(0) == 8 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c19d20_TEST FAIL\n"); return 1; }
    std::printf("00c19d20_TEST PASS\n");
    return 0;
}