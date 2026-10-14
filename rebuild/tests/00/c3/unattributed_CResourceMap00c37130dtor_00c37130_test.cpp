#include <cstdio>
struct X; static int g_res = 0, g_map = 0; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(X* m) { removed = m; } X* removed; Rg() : removed(0) {} }; struct Ow { unsigned char p[8]; Rg r; };
struct Rs { ~Rs() { ++g_res; } }; struct Nd { unsigned char b[0x28]; ~Nd() { ++g_map; } }; struct Rt { virtual void v0() {} Nd* pn; ~Rt() {} }; struct Ct : Rt { Nd n; ~Ct() {} };
struct X { Ow* o; long a, b; Rs* r; Ct e; ~X() { if (r) { delete r; r = 0; } o->r.Remove(this); } };
static bool Check() { Ow o; X* x = new X; x->o = &o; x->r = new Rs; delete x; return g_res == 1 && g_map == 1 && o.r.removed == x; }

int main()
{
    if (!Check()) { std::printf("00c37130_TEST FAIL\n"); return 1; }
    std::printf("00c37130_TEST PASS\n");
    return 0;
}