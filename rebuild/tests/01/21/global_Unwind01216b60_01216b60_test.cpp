#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
static int g_add = 0, g_rel = 0, g_on = 0, g_is = 0; static long g_cmd = 0; static void* g_tg = 0; struct Hd { virtual void v0() {} long a, b; }; struct It { virtual long QI() { return 0; } virtual unsigned long AddRef() { return ++g_add; } virtual unsigned long Release() { return ++g_rel; } unsigned char p[8]; void* buf; }; struct Src : Hd, It {};
struct X { virtual void OnStart() { ++g_on; } unsigned char p[0x5c]; long qd; unsigned char q[0x14]; Src* src; unsigned char r[0x18]; unsigned char sec[0x18]; long Issue(long c, It* t, long f) { ++g_is; g_cmd = c; g_tg = t; return 0; } long Start() { Lk l(sec); if (!src->buf) return 0; It* t = src; src->AddRef(); Issue(0x16, t, 0); qd = 1; OnStart(); src->Release(); return 1; } };
static bool Check() { Src s; s.buf = 0; X* x = new X; x->src = &s; x->qd = 0; if (x->Start() != 0 || g_add || x->qd) return false; int b; s.buf = &b; bool ok = x->Start() == 1 && g_add == 1 && g_rel == 1 && g_on == 1 && g_is == 1 && g_cmd == 0x16 && g_tg == static_cast<It*>(&s) && x->qd == 1 && g_enter == 2 && g_leave == 2; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00ca7120_TEST FAIL\n"); return 1; }
    std::printf("01216b60_TEST PASS\n");
    return 0;
}