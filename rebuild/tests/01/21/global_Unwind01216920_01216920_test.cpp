#include <cstdio>
static int g_e = 0, g_v = 0; struct CS; static void __stdcall E(CS*) { ++g_e; } static void __stdcall Lv(CS*) { ++g_v; } struct Lk { CS* p; Lk(CS* s) : p(s) { E(s); } ~Lk() { Lv(p); } };
static long g_n = -1; static int g_o = 0; static void __stdcall Nt(long s) { g_n = s; } struct X { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {} virtual void g() {} virtual void h() {} virtual void i() {} virtual void j() {} virtual void k() {} virtual void l() {} virtual void m() {} virtual void n() {} virtual void o() {} virtual void p() {} virtual void q() {} virtual void O() { ++g_o; } unsigned char x[0x60]; long st; unsigned char y[0xc]; long pd; unsigned char z[0x1c]; unsigned char cs[4]; long S() { Lk l((CS*)cs); pd = 0; if (st == 1) { st = 0; O(); Nt(1); } return 0; } };
static bool Check() { X x; x.st = 1; x.pd = 5; return x.S() == 0 && x.st == 0 && x.pd == 0 && g_o == 1 && g_n == 1 && g_e == 1 && g_v == 1; }

int main()
{
    if (!Check()) { std::printf("00ca4e30_TEST FAIL\n"); return 1; }
    std::printf("01216920_TEST PASS\n");
    return 0;
}