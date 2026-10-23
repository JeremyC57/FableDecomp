#include <cstdio>
#include <new>
static int g_node = 0; struct Nd { unsigned char b[0x28]; Nd(long k) { ++g_node; } ~Nd() { --g_node; } }; struct Rt { virtual void v0() {} Nd* pn; Rt(Nd* n) : pn(n) {} Rt() : pn(0) {} ~Rt() {} }; struct Ct : Rt { Nd n; Ct() : Rt(&n), n(0) {} ~Ct() {} };
struct X; static int g_c = 0, g_d = 0; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(X* o) { removed = o; } X* removed; Rg() : removed(0) {} }; struct Ow { unsigned char p[0x28]; Rg r; };
struct X { Ow* o; long a; Ct f; Ct s; void Cleanup() { ++g_c; } void Drain() { ++g_d; } ~X() { Cleanup(); Drain(); o->r.Remove(this); } };
static bool Check() { Ow o; X* x = new X; x->o = &o; delete x; return g_c == 1 && g_d == 1 && o.r.removed == x && g_node == 0; }

int main()
{
    if (!Check()) { std::printf("00c473f0_TEST FAIL\n"); return 1; }
    std::printf("012156be_TEST PASS\n");
    return 0;
}