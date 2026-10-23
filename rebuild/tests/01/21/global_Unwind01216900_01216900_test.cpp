#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
static int g_sleep = 0, g_set = 0, g_prime = 0; static long g_disp = 0, g_run = 11; static void Sleep(unsigned long m) { ++g_sleep; } static long SetEvent(void* h) { ++g_set; return 1; }
struct X { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void Prime() { ++g_prime; } virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual long Dispatch(void* w) { return g_disp; } virtual void v22() {} virtual void v23() {} virtual long Run() { return g_run; } unsigned char p[0x50]; void* ev; unsigned char q[0xc]; long st; unsigned char r[4]; void* wk; unsigned char s[0x24]; unsigned char sec[0x18];
 long Start() { Lk l(sec); if (st != 1) { st = 1; Sleep(1); Prime(); if (!wk) return Run(); if (Dispatch(wk) == 0) SetEvent(ev); } return 0; } };
static bool Check() { X* x = new X; x->st = 0; x->wk = 0; int w; x->ev = &w; if (x->Start() != 11) return false; if (x->Start() != 0 || g_prime != 1) return false; x->st = 0; x->wk = &w; g_disp = 0; x->Start(); bool ok = g_set == 1 && g_sleep == 2 && g_prime == 2 && g_enter == 3 && g_leave == 3; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00ca4d80_TEST FAIL\n"); return 1; }
    std::printf("01216900_TEST PASS\n");
    return 0;
}