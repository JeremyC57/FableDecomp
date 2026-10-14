#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
static int g_is = 0, g_rs = 0; static long g_cmd = 0; struct Hd { long a, b, c; }; struct Tg { unsigned char p[0xc]; void* buf; unsigned char q[0x85]; bool loop; }; struct Src : Hd, Tg {};
struct X { unsigned char p[0x60]; void* qd; unsigned char q[0xc]; void* pend; long m74; Src* src; unsigned char r[0x18]; unsigned char sec[0x18]; unsigned char s[4]; long st; void Issue(long c, Tg* t, long f) { ++g_is; g_cmd = c; } void Reset(long m) { ++g_rs; } void Chk() { Lk l(sec); if (!qd && src->buf && !src->loop && !pend && st == 1) { Issue(5, src, 0); Reset(0); } } };
static bool Check() { Src s; int b; s.buf = &b; s.loop = false; X* x = new X; x->qd = 0; x->pend = 0; x->src = &s; x->st = 1; x->Chk(); s.loop = true; x->Chk(); bool ok = g_is == 1 && g_rs == 1 && g_cmd == 5 && g_enter == 2 && g_leave == 2; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00ca7080_TEST FAIL\n"); return 1; }
    std::printf("00ca7080_TEST PASS\n");
    return 0;
}
