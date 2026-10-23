#include <cstdio>
#include <new>
static int g_base = 0, g_timer = 0, g_nb = 0, g_reg = 0; static long g_cap = 0, g_t0 = -1, g_t1 = -1; struct X; struct Bs { unsigned char b[0x18]; Bs() { ++g_base; } ~Bs() {} }; struct Sl { long m0; void* f; void* l; void* e; Sl() : f(0), l(0), e(0) {} ~Sl() {} }; struct Tm { long m0; Tm(long a, long b) { ++g_timer; g_t0 = a; g_t1 = b; } ~Tm() {} };
struct Nb { virtual void v0() {} unsigned char b[0xc]; Nb(long c) { ++g_nb; g_cap = c; } }; struct Nd : Nb { void* t; Tm* tm; X* o; Sl* s; Nd(void* T, Tm* M, X* O, Sl* S) : Nb(0x3000), t(T), tm(M), o(O), s(S) {} void Register() { ++g_reg; } };
struct X : Bs { Sl sl; Tm tm; Nd* nd; X(void* t) : Bs(), sl(), tm(0, 30000), nd(0) { nd = new Nd(t, &tm, this, &sl); nd->Register(); } };
static bool Check() { int t; X* x = new X(&t); bool ok = g_base == 1 && g_timer == 1 && g_t0 == 0 && g_t1 == 30000 && g_nb == 1 && g_cap == 0x3000 && g_reg == 1 && x->nd && x->nd->t == &t && x->nd->tm == &x->tm && x->nd->o == x && x->nd->s == &x->sl && x->sl.f == 0; delete x->nd; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c27070_TEST FAIL\n"); return 1; }
    std::printf("01213368_TEST PASS\n");
    return 0;
}