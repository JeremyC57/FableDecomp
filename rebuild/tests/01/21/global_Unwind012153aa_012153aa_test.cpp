#include <cstdio>
#include <new>
static int g_node = 0; struct Nd { unsigned char b[0x28]; Nd() { ++g_node; } ~Nd() { --g_node; } }; struct Rt { virtual void v0() {} Nd* pn; Rt() : pn(0) {} ~Rt() {} }; struct Ct : Rt { Nd n; Ct() : Rt(), n() { pn = &n; } ~Ct() {} };
struct X; static int g_a = 0, g_b = 0, g_first = 0, g_last = 0; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(X* f) { removed = f; } X* removed; Rg() : removed(0) {} }; struct Ow { unsigned char p[0x14]; Rg r; };
struct Fi { unsigned char b[0x18]; ~Fi() throw() { ++g_first; } }; struct La { unsigned char b[0x10]; ~La() { ++g_last; } };
struct X { Ow* o; long m4; Fi f; Ct s; Ct t; La l; void A() { ++g_a; } void B() { ++g_b; } ~X() { A(); B(); o->r.Remove(this); } };
static bool Check() { Ow o; X* x = new X; x->o = &o; delete x; return g_a == 1 && g_b == 1 && o.r.removed == x && g_node == 0 && g_first == 1 && g_last == 1; }

int main()
{
    if (!Check()) { std::printf("00c44f80_TEST FAIL\n"); return 1; }
    std::printf("012153aa_TEST PASS\n");
    return 0;
}