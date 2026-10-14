#include <cstdio>
#include <new>
#include <cstdlib>
static long g_err = 0; static int g_rep = 0; static long g_repv = 0; static void Rep(long e) { ++g_rep; g_repv = e; } typedef void (*PFN)(long); static PFN g_pfn = Rep;
static int g_take = 0, g_touch = 0, g_reg = 0, g_disc = 0, g_new = 0; static bool g_fail = false; static bool g_init = true; static long g_initv = 0;
struct Ac { virtual void v0() {} unsigned char p[0x44]; long m48; bool Init() { return g_init; } }; static Ac g_pool_a; struct Ab { virtual void v0() {} unsigned char p[0x44]; Ab() { ++g_new; } }; struct Ma : Ab { long m48; Ma() : Ab(), m48(0x80000000) {} static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct Pl { unsigned char b[0x30]; Ac* Take() { ++g_take; return g_take == 2 ? 0 : &g_pool_a; } void Touch() { ++g_touch; } void Reg(Ac* a) { ++g_reg; } };
struct X { unsigned char p[0x44]; Pl pool; long budget; void Disc(Ac* a) { ++g_disc; } Ac* Create() { Ac* a = 0; if (budget > 0) a = pool.Take(); else if (budget == -1) a = (Ac*)new Ma; else if (g_err == 0) { g_err = -9; if (g_pfn) g_pfn(-9); } if (!a) return 0; if (!a->Init()) { Disc(a); a = 0; } if (budget > 0) pool.Touch(); pool.Reg(a); return a; } };
static bool Check() { X* x = new X; x->budget = 1; if (x->Create() != &g_pool_a || g_take != 1 || g_touch != 1 || g_reg != 1) return false; if (x->Create() != 0 || g_take != 2 || g_reg != 1) return false; g_init = false; g_initv = 5; if (x->Create() != 0 || g_disc != 1 || g_reg != 2) return false; g_init = true; g_initv = 0; x->budget = -1; Ac* m = x->Create(); if (!m || g_new != 1 || g_reg != 3) return false; std::free(m); x->budget = 0; if (x->Create() != 0 || g_err != -9 || g_rep != 1 || g_repv != -9) return false; delete x; return true; }

int main()
{
    if (!Check()) { std::printf("00c3dcd0_TEST FAIL\n"); return 1; }
    std::printf("00c3dcd0_TEST PASS\n");
    return 0;
}