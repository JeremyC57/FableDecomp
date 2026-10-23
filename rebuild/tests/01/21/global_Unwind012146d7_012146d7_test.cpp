#include <cstdio>
#include <new>
static int g_node = 0, g_rel = 0, g_mem = 0, g_n = 2; struct X; struct En { long m0; void* pr; }; static En g_en; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(X* t) { removed = t; } X* removed; Rg() : removed(0) {} };
struct Wp { unsigned char p[0x104]; Rg r; }; struct Wi { unsigned char p[0x38]; Wp* part; }; struct W { long m0; Wi* inner; }; struct Ow { W* w; Rg r4; unsigned char p[0x2c]; Rg r34; };
struct Nd { unsigned char b[0x28]; ~Nd() { ++g_node; } }; struct Cr { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual En* Next() { return g_n-- > 0 ? &g_en : 0; } Nd* pn; ~Cr() {} }; struct Ct : Cr { Nd n; ~Ct() {} }; struct Mb { unsigned char b[0x20]; ~Mb() { ++g_mem; } };
struct X { virtual void v0() {} Ow* o; Mb m8; Ct pr; void Release(void* p) { if (p == g_en.pr) ++g_rel; } ~X() { En* e = pr.Next(); while (e) { Release(e->pr); e = pr.Next(); } o->r34.Remove(this); o->w->inner->part->r.Remove(this); o->r4.Remove(this); } };
static bool Check() { int p; g_en.pr = &p; Wp wp; Wi wi; wi.part = &wp; W w; w.inner = &wi; Ow o; o.w = &w; X* x = new X; x->o = &o; delete x; return g_rel == 2 && g_node == 1 && g_mem == 1 && o.r34.removed == x && wp.r.removed == x && o.r4.removed == x; }

int main()
{
    if (!Check()) { std::printf("00c37da0_TEST FAIL\n"); return 1; }
    std::printf("012146d7_TEST PASS\n");
    return 0;
}