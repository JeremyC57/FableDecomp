#include <cstdio>
#include <new>
static int g_base = 0, g_ev = 0, g_init = 0, g_set = 0; static long g_mr[3]; static void* g_sec = 0; static void* g_a = 0; static void* g_sig = 0;
static void Init(void* s) { ++g_init; } static void Set(void* h) { ++g_set; g_sig = h; }
struct Ir { virtual void v0() {} }; struct En { unsigned char p[8]; }; struct Eh { void* h; Eh(long m) { g_mr[g_ev] = m; h = (void*)(long)(100 + g_ev); ++g_ev; } ~Eh() {} }; struct Sc { unsigned char s[0x18]; Sc() { Init(s); } ~Sc() {} };
struct B0 { virtual ~B0() {} long a, b; B0(void* bb, void* c, void* s, void* aa) { ++g_base; g_sec = s; g_a = aa; } }; struct B1 { virtual void v0() {} }; struct B2 { virtual void v0() {} };
struct V : B0, B1, B2 { unsigned char p[0x50 - 0x14]; En* e; Eh h1, h2, h3; long m60, m64, m68, m6c, m70, m74; Ir* r; Sc s1, s2; long mac, mb0, mb4, mb8, mbc, mc0; Sc s3;
  V(void* a, void* bb, void* c, void* d) : B0(bb, c, s1.s, a), e(0), h1(0), h2(1), h3(1), m60(0), m64(0), m68(0), m6c(0), m70(0), m74(0), r(0), s1(), s2(), mac(0), mb0(1), mb4(0), mb8(0), mbc(0), mc0(0), s3() { Set(h3.h); } virtual ~V() {} };
static bool Check() { int a, b, c, d; V* v = new V(&a, &b, &c, &d); bool ok = g_base == 1 && g_sec == v->s1.s && g_a == &a && v->e == 0 && g_ev == 3 && g_mr[0] == 0 && g_mr[1] == 1 && g_mr[2] == 1 && v->m60 == 0 && v->m74 == 0 && v->r == 0 && g_init == 3 && v->mac == 0 && v->mb0 == 1 && v->mc0 == 0 && g_set == 1 && g_sig == v->h3.h; delete v; return ok; }

int main()
{
    if (!Check()) { std::printf("00ca61d0_TEST FAIL\n"); return 1; }
    std::printf("012169c0_TEST PASS\n");
    return 0;
}