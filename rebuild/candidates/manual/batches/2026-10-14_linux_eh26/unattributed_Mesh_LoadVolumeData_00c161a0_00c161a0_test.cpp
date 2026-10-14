#include <cstdio>
#include <new>
static int g_lk = 0, g_as = 0, g_rel = 0, g_add = 0; static float g_pos = 0; static long g_c = 0, g_10 = 0, g_14 = 0, g_18 = 0; static float g_sc = 0;
struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } }; struct Bf { void* d; long n; void Assign(const Bf* o) { ++g_as; d = o->d; n = o->n; } void Release() { ++g_rel; d = 0; } };
struct Rc { float sc; Bf b; long c, m10, m14, m18; Rc() { sc = 0; b.d = 0; b.n = 0; c = 1; m10 = 0; m14 = 0; m18 = 0x3f800000; } ~Rc() { b.Release(); } }; struct Sr { long m0, m4, m8, mc; unsigned long m10, m14; };
struct Vl { long Add(float p, Rc* r) { ++g_add; g_pos = p; g_sc = r->sc; g_c = r->c; g_10 = r->m10; g_14 = r->m14; g_18 = r->m18; return 0; } }; static unsigned char g_sec[8];
struct X { unsigned char p[8]; Vl* v; void Load(const Bf* b, const Sr* s) { Pl l(g_sec); Rc r; r.sc = (float)s->m10; r.b.Assign(b); r.c = s->m4; r.m10 = s->m0; r.m14 = s->mc; r.m18 = s->m8; v->Add((float)s->m14, &r); } };
static bool Check() { Vl v; X x; x.v = &v; Bf b; int d; b.d = &d; b.n = 3; Sr s; s.m0 = 10; s.m4 = 11; s.m8 = 12; s.mc = 13; s.m10 = 0x80000000u; s.m14 = 5; x.Load(&b, &s); return g_add == 1 && g_pos == 5.0f && g_sc == 2147483648.0f && g_c == 11 && g_10 == 10 && g_14 == 13 && g_18 == 12 && g_as == 1 && g_rel == 1 && g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c161a0_TEST FAIL\n"); return 1; }
    std::printf("00c161a0_TEST PASS\n");
    return 0;
}
