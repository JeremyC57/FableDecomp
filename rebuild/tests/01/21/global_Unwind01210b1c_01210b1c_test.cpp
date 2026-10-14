#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Rq { virtual void v0() {} long v; Rq(void* s) : v(s ? 1 : 0) {} ~Rq() {} }; struct Pc {}; static Pc g_pc; static void* g_a = 0; static void* g_ctx = 0; static long g_rv = 0;
static bool Proc(Pc* p, void* a, void* c, Rq* r) { g_a = a; g_ctx = c; g_rv = r->v; return true; }
struct X { unsigned char p[0x5c]; Pc* pc; unsigned char q[0x20]; void* ctx; bool P(void* a, void* b) { L l(&g_g); Rq r(b); return Proc(pc, a, ctx, &r); } };
static bool Check() { X* x = new X; x->pc = &g_pc; int c, a, b; x->ctx = &c; bool ok = x->P(&a, &b) && g_a == &a && g_ctx == &c && g_rv == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c09110_TEST FAIL\n"); return 1; }
    std::printf("00c09110_TEST PASS\n");
    return 0;
}