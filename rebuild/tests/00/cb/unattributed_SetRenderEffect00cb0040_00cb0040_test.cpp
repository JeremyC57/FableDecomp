#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0, g_add = 0, g_rel = 0, g_det = 0, g_rb = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; }
struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } }; struct IE { virtual long QI() { return 0; } virtual unsigned long AddRef() { return ++g_add; } virtual unsigned long Release() { return ++g_rel; } virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual long Detach(void* t) { ++g_det; return 0; } };
struct X { unsigned char p[8]; unsigned char sec[0x18]; unsigned char q[0x34]; void* pend; unsigned char r[8]; IE* eff; void Rebuild() { ++g_rb; } long Set(IE* e) { Lk l(sec); if (e) e->AddRef(); if (eff) { if (pend) { eff->Detach(pend); pend = 0; } eff->Release(); } eff = e; Rebuild(); return 0; } };
static bool Check() { IE a, b; X x; x.pend = 0; x.eff = 0; x.Set(&a); int t; x.pend = &t; x.Set(&b); x.Set(0); return g_enter == 3 && g_leave == 3 && g_add == 2 && g_rel == 2 && g_det == 1 && g_rb == 3 && x.eff == 0 && x.pend == 0; }

int main()
{
    if (!Check()) { std::printf("00cb0040_TEST FAIL\n"); return 1; }
    std::printf("00cb0040_TEST PASS\n");
    return 0;
}