#include <cstdio>
#include <new>
static int g_node = 0; struct Nd { unsigned char b[0x28]; Nd(long k) { ++g_node; } ~Nd() { --g_node; } }; struct Rt { virtual void v0() {} Nd* pn; Rt(Nd* n) : pn(n) {} Rt() : pn(0) {} ~Rt() {} }; struct Ct : Rt { Nd n; Ct() : Rt(&n), n(0) {} ~Ct() {} };
struct X; static int g_os = 0, g_cf = 0; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void Add(X* o) { added = o; } X* added; Rg() : added(0) {} }; struct Ow { unsigned char p[0x28]; Rg r; };
struct X { Ow* o; void* d; Ct f; Ct s; void Open() { ++g_os; } void Create() { ++g_cf; } X(Ow* O, void* D) : o(O), d(D) { o->r.Add(this); Open(); Create(); } };
static bool Check() { Ow o; int d; X x(&o, &d); return o.r.added == &x && g_os == 1 && g_cf == 1 && g_node == 2 && x.f.pn == &x.f.n && x.s.pn == &x.s.n; }

int main()
{
    if (!Check()) { std::printf("00c47340_TEST FAIL\n"); return 1; }
    std::printf("00c47340_TEST PASS\n");
    return 0;
}