#include <cstdio>
#include <new>
static int g_part = 0, g_node = 0, g_add = 0, g_i1 = 0, g_i2 = 0; static void* g_pa = 0; static void* g_pc = 0;
struct St; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void Add(St* s) { ++g_add; } };
struct Wp { unsigned char p[0x104]; Rg r; }; struct Wl { unsigned char p[0x38]; Wp* part; }; struct Wd { unsigned char p[4]; Wl* link; }; struct Ow { Wd* w; Rg r; unsigned char p[0x2c]; Rg r2; };
struct Bd { long a, b, c, d; }; struct Pt { unsigned char b[8]; Pt(void* A, void* C) { ++g_part; g_pa = A; g_pc = C; } ~Pt() {} }; struct Nd { unsigned char b[0x28]; Nd(long m) { ++g_node; } ~Nd() {} }; struct Rt { virtual void v0() {} Nd* pn; Rt(Nd* n) : pn(n) {} ~Rt() {} }; struct Ct : Rt { Nd n; Ct() : Rt(&n), n(0) {} ~Ct() {} };
struct St { virtual void v0() {} Ow* o; Pt pt; void* m10; Bd bd; bool m24; Ct e; void* m58; long fl; void I1() { ++g_i1; } void I2() { ++g_i2; } St(Ow* O, void* a, void* b, void* c, void* d, const Bd* B, bool fa, bool fb) : o(O), pt(a, c), m10(d), bd(*B), m24(false), e(), m58(b), fl((fa ? 1 : 0) | (fb ? 2 : 0)) { o->r.Add(this); o->w->link->part->r.Add(this); o->r2.Add(this); I1(); I2(); } };
static bool Check() { Wp wp; Wl wl; wl.part = &wp; Wd wd; wd.link = &wl; Ow o; o.w = &wd; Bd b = { 1, 2, 3, 4 }; int a, c, d, x; St* s = new St(&o, &a, &x, &c, &d, &b, true, false); bool ok = s->o == &o && g_part == 1 && g_pa == &a && g_pc == &c && s->m10 == &d && s->bd.d == 4 && !s->m24 && g_node == 1 && s->m58 == &x && s->fl == 1 && g_add == 3 && g_i1 == 1 && g_i2 == 1; delete s; St* t = new St(&o, &a, &x, &c, &d, &b, false, true); ok = ok && t->fl == 2; delete t; St* u = new St(&o, &a, &x, &c, &d, &b, true, true); ok = ok && u->fl == 3; delete u; return ok; }

int main()
{
    if (!Check()) { std::printf("00c37c90_TEST FAIL\n"); return 1; }
    std::printf("00c37c90_TEST PASS\n");
    return 0;
}
