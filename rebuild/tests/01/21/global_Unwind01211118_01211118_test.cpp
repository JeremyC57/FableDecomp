#include <cstdio>
#include <new>
static int g_nav = 0, g_sn = 0, g_bn = 0, g_add = 0; static long g_mode = -1; struct X; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void Add(X* d) { ++g_add; } }; struct Ow { unsigned char p[4]; Rg r; };
struct Bs { virtual void v0() {} Bs() {} ~Bs() {} }; struct Nv { unsigned char b[0x20]; Nv() { ++g_nav; } ~Nv() {} }; struct Bf { void* p; long a, b; Bf() : p(0), a(0), b(0) {} ~Bf() {} }; struct Sn { unsigned char b[8]; Sn(long m) { ++g_sn; g_mode = m; } ~Sn() {} }; struct Bn { unsigned char b[0x28]; Bn(long m) { ++g_bn; } ~Bn() {} };
struct Ra { virtual void v0() {} Sn* pn; Ra(Sn* n) : pn(n) {} ~Ra() {} }; struct Rb { virtual void v0() {} Sn* pn; Rb(Sn* n) : pn(n) {} ~Rb() {} }; struct Rc { virtual void v0() {} Sn* pn; Rc(Sn* n) : pn(n) {} ~Rc() {} }; struct Rd { virtual void v0() {} Bn* pn; Rd(Bn* n) : pn(n) {} ~Rd() {} };
struct Ca : Ra { Sn n; Ca() : Ra(&n), n(1) {} ~Ca() {} }; struct Cb : Rb { Sn n; Cb() : Rb(&n), n(1) {} ~Cb() {} }; struct Cc : Rc { Sn n; Cc() : Rc(&n), n(1) {} ~Cc() {} }; struct Cd : Rd { Bn n; Cd() : Rd(&n), n(0) {} ~Cd() {} };
struct X : Bs { long m4; Ow* o; Nv nv; Bf bf; Ca a; long m48, m4c; Cb b; Cc c; long m70; Cd d; X(Ow* O) : m4(0), o(O), nv(), bf(), a(), m48(0), m4c(1), b(), c(), m70(-1), d() { o->r.Add(this); } };
static bool Check() { Ow o; X* x = new X(&o); bool ok = x->m4 == 0 && x->o == &o && g_nav == 1 && x->bf.p == 0 && x->bf.b == 0 && g_sn == 3 && g_mode == 1 && g_bn == 1 && x->m48 == 0 && x->m4c == 1 && x->m70 == -1 && x->a.pn == &x->a.n && x->d.pn == &x->d.n && g_add == 1; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0edf0_TEST FAIL\n"); return 1; }
    std::printf("01211118_TEST PASS\n");
    return 0;
}