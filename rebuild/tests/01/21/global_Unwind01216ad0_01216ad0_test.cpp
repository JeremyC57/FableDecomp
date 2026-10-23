#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
#include <cstdlib>
struct Host; static int g_ctor = 0, g_dtor = 0; static long g_err = 0; static Host* g_h = 0; struct Gr { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual ~Gr() { ++g_dtor; } unsigned char b[0xdc]; Gr(Host* h, long* e, const char* n) { ++g_ctor; g_h = h; *e = g_err; } };
struct Host { unsigned char p[0x78]; Gr* g; unsigned char q[0x48]; unsigned char sec[0x18]; Gr* Get(long mode) { Lk l(sec); if (mode != 0) return 0; if (g == 0) { long err = 0; g = new Gr(this, &err, "I"); if (g == 0) return 0; if (err < 0) { delete g; g = 0; return 0; } } return g; } };
static bool Check() { Host* h = new Host; h->g = 0; if (h->Get(1) != 0 || g_ctor) return false; g_err = -1; if (h->Get(0) != 0 || g_ctor != 1 || g_dtor != 1 || h->g) return false; g_err = 0; Gr* g = h->Get(0); if (!g || g_ctor != 2 || h->g != g || g_h != h) return false; bool ok = h->Get(0) == g && g_ctor == 2 && g_enter == 4 && g_leave == 4; delete g; delete h; return ok; }

int main()
{
    if (!Check()) { std::printf("00ca6a30_TEST FAIL\n"); return 1; }
    std::printf("01216ad0_TEST PASS\n");
    return 0;
}