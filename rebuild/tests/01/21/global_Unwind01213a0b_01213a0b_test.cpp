#include <cstdio>
#include <new>
static int g_node = 0, g_h = 0, g_n = 2; struct X; struct Ev {}; static Ev g_ev; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(X* s) { removed = s; } X* removed; Rg() : removed(0) {} }; struct Ow { unsigned char p[0x10]; Rg r; };
struct Sr { virtual void v0() {} Ow* o; ~Sr() {} }; struct Nd { unsigned char b[0x28]; ~Nd() { ++g_node; } }; struct Cr { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual Ev* Next() { return g_n-- > 0 ? &g_ev : 0; } Nd* pn; ~Cr() {} }; struct Ct : Cr { Nd n; ~Ct() {} }; struct Li { virtual void v0() {} unsigned char b[0xc]; ~Li() {} };
struct X : Sr { Ct ev; Li li; void H(Ev* e) { ++g_h; } ~X() { Ev* e = ev.Next(); while (e) { H(e); e = ev.Next(); } o->r.Remove(this); } };
static bool Check() { Ow o; X* x = new X; x->o = &o; delete x; return g_h == 2 && g_node == 1 && o.r.removed == x; }

int main()
{
    if (!Check()) { std::printf("00c2cc60_TEST FAIL\n"); return 1; }
    std::printf("01213a0b_TEST PASS\n");
    return 0;
}