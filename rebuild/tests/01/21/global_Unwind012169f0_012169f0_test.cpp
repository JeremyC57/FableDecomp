#include <cstdio>
#include <new>
static int g_sh = 0, g_fi = 0, g_rel = 0, g_del = 0, g_hd = 0, g_bs = 0; static long g_fl = 0; static void Del(void* s) { ++g_del; }
struct Ir { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void Rel(long f) { ++g_rel; g_fl = f; } }; struct En { unsigned char p[8]; Ir r; }; struct Hd { void* p; ~Hd() { ++g_hd; } }; struct Sc { unsigned char s[0x18]; ~Sc() { Del(s); } };
struct B0 { virtual ~B0() { ++g_bs; } long a, b; }; struct B1 { virtual void v0() {} }; struct B2 { virtual void v0() {} };
struct V : B0, B1, B2 { unsigned char p[0x50 - 0x14]; En* e; Hd h1, h2, h3; unsigned char q[0x78 - 0x60]; Ir* r; Sc s1, s2; unsigned char z[0xc4 - 0xac]; Sc s3; void Sh() { ++g_sh; } void Fi() { ++g_fi; } virtual ~V() { Sh(); Fi(); if (e) { e->r.Rel(1); e = 0; } if (r) { r->Rel(1); r = 0; } } };
static bool Check() { En en; Ir ir; V* v = new V; v->e = &en; v->r = &ir; delete v; if (g_sh != 1 || g_fi != 1 || g_rel != 2 || g_fl != 1 || g_del != 3 || g_hd != 3 || g_bs != 1) return false; V* w = new V; w->e = 0; w->r = 0; delete w; return g_rel == 2 && g_del == 6; }

int main()
{
    if (!Check()) { std::printf("00ca6370_TEST FAIL\n"); return 1; }
    std::printf("012169f0_TEST PASS\n");
    return 0;
}