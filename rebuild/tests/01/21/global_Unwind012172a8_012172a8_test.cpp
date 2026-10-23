#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
#include <cstdlib>
struct Pa; static int g_ctor = 0; static long g_res = 0; static double g_sc = 0; static void* g_i = 0; static long g_zero = -1; static bool g_fail = false;
struct Tk { unsigned char b[0x50]; Tk(Pa* o, long z, long* r, void* a, double s, void* b2, void* c, void* d, void* e, void* f, void* g, void* h, void* i) { ++g_ctor; g_zero = z; *r = g_res; g_sc = s; g_i = i; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct Pa { long m0, m4; unsigned char sec[0x18]; long Alloc(Tk** out, void* a, double s, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i) { Lk l(sec); long res = 0; *out = 0; Tk* t = new Tk(this, 0, &res, a, s, b, c, d, e, f, g, h, i); long hr; if (!t) hr = 0x8007000EL; else { hr = res; *out = t; } return hr; } };
static bool Check() { Pa p; Tk* t = (Tk*)1; int i; g_fail = true; if (p.Alloc(&t, 0, 1.5, 0, 0, 0, 0, 0, 0, 0, &i) != 0x8007000EL || t != 0 || g_ctor) return false; g_fail = false; g_res = 7; long hr = p.Alloc(&t, 0, 2.5, 0, 0, 0, 0, 0, 0, 0, &i); bool ok = hr == 7 && t != 0 && g_ctor == 1 && g_zero == 0 && g_sc == 2.5 && g_i == &i && g_enter == 2 && g_leave == 2; delete t; return ok; }

int main()
{
    if (!Check()) { std::printf("00caf680_TEST FAIL\n"); return 1; }
    std::printf("012172a8_TEST PASS\n");
    return 0;
}