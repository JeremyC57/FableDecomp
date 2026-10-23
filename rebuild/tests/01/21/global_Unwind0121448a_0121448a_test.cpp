#include <cstdio>
#include <new>
static int g_node = 0, g_rel = 0, g_n = 2; struct X; struct En { long m0; void* res; }; static En g_en; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(X* o) { removed = o; } X* removed; Rg() : removed(0) {} }; struct W { unsigned char p[0x68]; Rg a; unsigned char q[0x2c]; Rg b; }; struct Ow { W* w; unsigned char p[0xc]; Rg r; unsigned char q[0x38]; bool cleared; };
struct Nd { unsigned char b[0x28]; ~Nd() { ++g_node; } }; struct Cr { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual En* Next() { return g_n-- > 0 ? &g_en : 0; } Nd* pn; ~Cr() {} }; struct Ct : Cr { Nd n; ~Ct() {} };
struct X { Ow* o; long m4, m8, mc, m10; Ct e; void Release(void* r) { if (r == g_en.res) ++g_rel; } ~X() { En* en = e.Next(); while (en) { Release(en->res); en = e.Next(); } o->cleared = true; o->w->a.Remove(this); o->w->b.Remove(this); o->r.Remove(this); } };
static bool Check() { int r; g_en.res = &r; W w; Ow o; o.w = &w; o.cleared = false; X* x = new X; x->o = &o; delete x; return g_rel == 2 && g_node == 1 && o.cleared && w.a.removed == x && w.b.removed == x && o.r.removed == x; }

int main()
{
    if (!Check()) { std::printf("00c36590_TEST FAIL\n"); return 1; }
    std::printf("0121448a_TEST PASS\n");
    return 0;
}