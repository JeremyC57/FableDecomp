// Behaviour test for 00c56370: CAudioContext::~CAudioContext (the landed test; the body under test is unchanged)
#include <cstdio>
#include <new>
#include <cstring>
static int g_mc = 0, g_dc = 0, g_gpa = 0, g_free = 0, g_gen = 0, g_mix = 0; static void* g_mcarg = (void*)1; static void* g_dcarg = 0; static void MakeCurrent(void* c) { ++g_mc; g_mcarg = c; } static void Destroy(void* c) { ++g_dc; g_dcarg = c; } typedef void (*Pfn)(void*); static void* GetProc(void* m, const char* n) { ++g_gpa; return std::strcmp(n, "alcDestroyContext") == 0 ? (void*)&Destroy : 0; } static long Free(void* m) { ++g_free; return 1; }
struct Bs { virtual void v0() {} ~Bs() {} }; struct Mx { unsigned char b[8]; ~Mx() { ++g_mix; } };
struct X : Bs { void* mod; long m8, mc; Pfn mk; long m14; Pfn ds; unsigned char p[0xe4]; void* ctx; unsigned char q[0xc]; Mx mx; void Gen() { ++g_gen; } ~X() { if (ctx) { mk(0); if (!ds) ds = (Pfn)GetProc(mod, "alcDestroyContext"); ds(ctx); ctx = 0; } Gen(); if (mod) { Free(mod); mod = 0; } } };
static bool Check() { int m, c; X* x = new X; x->mod = &m; x->mk = MakeCurrent; x->ds = 0; x->ctx = &c; delete x; if (!(g_mc == 1 && g_mcarg == 0 && g_gpa == 1 && g_dc == 1 && g_dcarg == &c && g_gen == 1 && g_free == 1 && g_mix == 1)) return false; X* y = new X; y->mod = 0; y->ctx = 0; y->ds = Destroy; delete y; return g_mc == 1 && g_dc == 1 && g_gen == 2 && g_free == 1 && g_mix == 2; }

int main()
{
    if (!Check()) { std::printf("00c56370_TEST FAIL\n"); return 1; }
    std::printf("00c56370_TEST PASS\n");
    return 0;
}