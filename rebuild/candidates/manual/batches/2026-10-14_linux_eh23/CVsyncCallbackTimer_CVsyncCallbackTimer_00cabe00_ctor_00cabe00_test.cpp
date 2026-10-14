#include <cstdio>
#include <new>
static int g_add = 0, g_info = 0, g_copy = 0; static long g_mode = -1; struct Hd { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual long GetPeriod() { return 16; } virtual long GetPhase() { return 3; } long a, b; }; struct It { virtual long QI() { return 0; } virtual unsigned long AddRef() { return ++g_add; } virtual unsigned long Release() { return 0; } }; struct Src : Hd, It {};
struct Inf { unsigned char b[0x18]; Inf(long m) { ++g_info; g_mode = m; } ~Inf() {} void CopyFrom(const Inf* o) { ++g_copy; } };
struct X { virtual void v0() {} long m4; long phase; Src* src; long period; long m14; Inf info; X(Src* s, const X* o) : m4(0), phase(0), src(s), m14(1), info(0) { src->AddRef(); if (!o) { period = src->GetPeriod(); phase = src->GetPhase(); return; } m4 = o->m4; phase = o->phase; period = o->period; info.CopyFrom(&o->info); } };
static bool Check() { Src s; X* a = new X(&s, 0); if (!(a->period == 16 && a->phase == 3 && a->m4 == 0 && a->m14 == 1 && g_add == 1 && g_info == 1 && g_mode == 0 && g_copy == 0)) return false; a->m4 = 7; X* b = new X(&s, a); bool ok = b->m4 == 7 && b->phase == 3 && b->period == 16 && g_add == 2 && g_copy == 1 && g_info == 2; delete a; delete b; return ok; }

int main()
{
    if (!Check()) { std::printf("00cabe00_TEST FAIL\n"); return 1; }
    std::printf("00cabe00_TEST PASS\n");
    return 0;
}
