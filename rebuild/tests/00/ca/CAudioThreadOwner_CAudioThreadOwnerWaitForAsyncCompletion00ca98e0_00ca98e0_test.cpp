#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0, g_waits = 0; static unsigned long g_r[4]; static int g_ri = 0; static long* g_pstate = 0; static void* g_h0 = 0; static void* g_h1 = 0;
static void En(void* s) { ++g_enter; } static void Le(void* s) { ++g_leave; } static unsigned long Wmo(unsigned long n, void* const* h, int all, unsigned long t) { ++g_waits; g_h0 = h[0]; g_h1 = h[1]; unsigned long r = g_r[g_ri++]; if (g_ri == 2 && g_pstate) *g_pstate = 1; return r; } static long LastErr() { return (long)0x80070006; }
struct Sl { void* s; Sl(void* x) : s(x) { En(s); } ~Sl() { Le(s); } };
struct O { unsigned char pad[0xa4]; unsigned char sec[0x18]; void* done; void* thr; long state; unsigned long tid; long cnt; void* abort; long Wait() { Sl l(sec); while (state == 2) { Le(sec); void* hs[2]; hs[0] = done; hs[1] = abort; unsigned long r = Wmo(2, hs, 0, 0xffffffff); En(sec); switch (r) { case 0: break; case 1: return (long)0x80040223; case 0xffffffff: return LastErr(); default: return (long)0x8000ffff; } } ++cnt; return 0; } };
static bool Check() { O* o = new O; o->state = 1; o->cnt = 0; o->done = (void*)0x11; o->abort = (void*)0x22; if (o->Wait() != 0 || o->cnt != 1 || g_waits) return false; o->state = 2; g_pstate = &o->state; g_r[0] = 0; g_r[1] = 0; if (o->Wait() != 0 || o->cnt != 2 || g_waits != 2 || o->state != 1 || g_h0 != (void*)0x11 || g_h1 != (void*)0x22) return false; g_pstate = 0; o->state = 2; g_ri = 0; g_r[0] = 1; if (o->Wait() != (long)0x80040223) return false; g_ri = 0; g_r[0] = 0xffffffff; if (o->Wait() != (long)0x80070006) return false; g_ri = 0; g_r[0] = 5; if (o->Wait() != (long)0x8000ffff || o->cnt != 2) return false; delete o; return g_enter == g_leave; }

int main()
{
    if (!Check()) { std::printf("00ca98e0_TEST FAIL\n"); return 1; }
    std::printf("00ca98e0_TEST PASS\n");
    return 0;
}