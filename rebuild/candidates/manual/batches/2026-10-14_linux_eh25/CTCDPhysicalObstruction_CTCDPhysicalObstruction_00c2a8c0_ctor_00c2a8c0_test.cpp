#include <cstdio>
#include <new>
static int g_node = 0; static long g_mode = -1; struct Nd { unsigned char b[0x28]; Nd(long m) { ++g_node; g_mode = m; } ~Nd() {} }; struct Rt { virtual void v0() {} Nd* pn; Rt(Nd* n) : pn(n) {} ~Rt() {} }; struct Ca : Rt { Nd n; Ca() : Rt(&n), n(0) {} ~Ca() {} }; struct Cb : Rt { Nd n; Cb() : Rt(&n), n(0) {} ~Cb() {} }; struct Cc : Rt { Nd n; Cc() : Rt(&n), n(0) {} ~Cc() {} }; struct Cd : Rt { Nd n; Cd() : Rt(&n), n(0) {} ~Cd() {} };
struct X { void* o; bool m4; Ca a; Cb b; Cc c; Cd d; long c8; bool cc, cd, ce; void* ctx; X(void* O, void* C) : o(O), m4(true), a(), b(), c(), d(), c8(0), cc(false), cd(false), ce(true), ctx(C) {} };
static bool Check() { int o, c; X* x = new X(&o, &c); bool ok = x->o == &o && x->m4 && x->a.pn == &x->a.n && x->d.pn == &x->d.n && g_node == 4 && g_mode == 0 && x->c8 == 0 && !x->cc && !x->cd && x->ce && x->ctx == &c; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c2a8c0_TEST FAIL\n"); return 1; }
    std::printf("00c2a8c0_TEST PASS\n");
    return 0;
}
