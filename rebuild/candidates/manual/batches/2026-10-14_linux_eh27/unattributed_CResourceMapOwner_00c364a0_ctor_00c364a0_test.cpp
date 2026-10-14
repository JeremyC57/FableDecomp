#include <cstdio>
#include <new>
static int g_node = 0, g_pair = 0, g_part = 0, g_add = 0, g_init = 0; static float g_sc = 0; static float g_val = 0; static void* g_ctx = 0;
struct Nd { unsigned char b[0x28]; Nd(long m) { ++g_node; } ~Nd() {} }; struct Rt { virtual void v0() {} Nd* pn; Rt(Nd* n) : pn(n) {} ~Rt() {} }; struct Ct : Rt { Nd n; Ct() : Rt(&n), n(0) {} ~Ct() {} };
struct Pr { float s; float v; Pr(float S, float V) { s = S; v = V; ++g_pair; } }; struct Pt { unsigned char b[0x114]; Pt(void* c) { ++g_part; g_ctx = c; } };
struct X; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void Add(X* o) { ++g_add; } }; struct Wd { unsigned char p[0x68]; Rg a; unsigned char q[0x2c]; Rg b; }; struct Ow { Wd* w; void* ctx; unsigned char p[8]; Rg r; unsigned char q[0x38]; bool dirty; };
struct X { Ow* o; long m4, m8, mc, m10; Ct e; Pr p; Pt pt; long m160; float m164; void Init() { ++g_init; } X(Ow* O, long a, long b, long c, float d, long e2, long f) : o(O), m4(a), m8(b), mc(e2), m10(f), e(), p(1.0f, d), pt(o->ctx), m160(c), m164(d) { o->dirty = true; o->r.Add(this); o->w->b.Add(this); o->w->a.Add(this); Init(); } };
static bool Check() { Wd w; Ow o; int c; o.w = &w; o.ctx = &c; o.dirty = false; X* x = new X(&o, 1, 2, 3, 4.5f, 6, 7); bool ok = x->o == &o && x->m4 == 1 && x->m8 == 2 && x->mc == 6 && x->m10 == 7 && x->m160 == 3 && x->m164 == 4.5f && x->p.s == 1.0f && x->p.v == 4.5f && g_node == 1 && g_pair == 1 && g_part == 1 && g_ctx == &c && g_add == 3 && g_init == 1 && o.dirty && x->e.pn == &x->e.n; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c364a0_TEST FAIL\n"); return 1; }
    std::printf("00c364a0_TEST PASS\n");
    return 0;
}
