#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Vb { virtual void v0() {} ~Vb() {} }; struct Pv : Vb { void* a; Pv(void* x) : a(x) {} }; struct Nv : Vb { void* a; Nv(void* x) : a(x) {} }; static Vb* g_pa = 0; static Vb* g_na = 0;
struct Pt { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void Accept(Vb* v) { g_pa = v; } }; struct Nd { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void Accept(Vb* v) { g_na = v; } unsigned char p[0xc]; Pt pt; }; static Nd g_nd;
struct X { Nd* Find(void* k) { return &g_nd; } long Visit(void* k, void* a) { L l(&g_g); Nd* n = Find(k); Pv pv(a); Nv nv(a); n->pt.Accept(&pv); n->Accept(&nv); return static_cast<Pv*>(g_pa)->a == a && static_cast<Nv*>(g_na)->a == a ? 0 : -1; } };
static bool Check() { X x; int a, k; return x.Visit(&k, &a) == 0 && g_pa && g_na && g_pa != g_na && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c19d90_TEST FAIL\n"); return 1; }
    std::printf("00c19d90_TEST PASS\n");
    return 0;
}
