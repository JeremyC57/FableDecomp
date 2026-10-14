#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } }; static void* g_v = 0; struct Tg { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void i() {} virtual void j() {} virtual void k() {} virtual void l() {} virtual void m() {} virtual long Q(void* v) { g_v = v; return 6; } }; struct X { unsigned char p[16]; Tg t; long Q(void* v) { L l(&g_g); return t.Q(v); } };
static bool Check() { X x; int i; return x.Q(&i) == 6 && g_v == &i && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c14d00_TEST FAIL\n"); return 1; }
    std::printf("00c14d00_TEST PASS\n");
    return 0;
}