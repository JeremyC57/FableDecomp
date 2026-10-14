#include <cstdio>
#include <new>
#include <cstdlib>
static int g_new = 0, g_del = 0, g_ctor = 0, g_dtor = 0, g_rel = 0, g_rep = 0; static long g_err = 0; static long g_reported = 0; static long g_phys = 1; static long g_last = 0; static bool g_fail = false; struct Ms { long HasPhysicsMesh() { return g_phys; } };
struct Ib { virtual ~Ib() { ++g_dtor; } unsigned char b[0xec]; Ib(long hp) { ++g_ctor; } long Resize() { return g_err; } void Release() { ++g_rel; } static void* operator new(size_t n) throw() { ++g_new; return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { ++g_del; std::free(p); } };
typedef void (*Rp)(long); static void Report(long e) { ++g_rep; g_reported = e; } static Rp g_pfn = 0;
struct X { long m0; Ms* ms; unsigned char p[0x10]; Ib* buf; Ib* Create() { Ib* b = new Ib(ms->HasPhysicsMesh()); if (!b) return 0; buf = b; long e = b->Resize(); if (e) { if (!g_last) { g_last = e; if (g_pfn) g_pfn(e); } b->Release(); delete b; b = 0; } return b; } };
static bool Check() { Ms ms; X x; x.ms = &ms; x.buf = 0; g_fail = true; if (x.Create() != 0 || g_ctor) return false; g_fail = false; g_err = 9; if (x.Create() != 0 || g_last != 9 || g_rep != 0 || g_rel != 1 || g_del != 1 || g_dtor != 1) return false; g_last = 0; g_pfn = Report; if (x.Create() != 0 || g_reported != 9 || g_rep != 1) return false; if (x.Create() != 0 || g_rep != 1) return false; g_err = 0; Ib* b = x.Create(); bool ok = b && x.buf == b && g_ctor == 4 && g_del == 3; delete b; return ok && g_del == 4; }

int main()
{
    if (!Check()) { std::printf("00c3df10_TEST FAIL\n"); return 1; }
    std::printf("00c3df10_TEST PASS\n");
    return 0;
}