#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
struct It { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual long Status() { return st; } unsigned char p[0x14]; void* h; long st; }; static It g_it[3];
struct Pt { long m0, m4, pend; unsigned char p[0x20]; void* sec; }; struct Hd { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual long Count() { return 3; } virtual It* GetAt(long i) { return &g_it[i]; } long a, b; }; struct Pm : Hd, Pt {};
static long Collect(Pt& part) { Lk l(part.sec); long r = 0; if (part.pend != 0) { Pm& pm = static_cast<Pm&>(part); long n = pm.Count(); for (long i = 0; i < n; ++i) { It* it = pm.GetAt(i); if (it->h) { long s = it->Status(); if (s < 0 && r >= 0) r = s; } } } part.pend = 0; return r; }
static bool Check() { int h; g_it[0].h = &h; g_it[0].st = 1; g_it[1].h = &h; g_it[1].st = -4; g_it[2].h = &h; g_it[2].st = -9; Pm pm; unsigned char sec[8]; pm.sec = sec; pm.pend = 1; long r = Collect(pm); return r == -4 && pm.pend == 0 && g_enter == 1 && g_leave == 1; }

int main()
{
    if (!Check()) { std::printf("00caab30_TEST FAIL\n"); return 1; }
    std::printf("00caab30_TEST PASS\n");
    return 0;
}