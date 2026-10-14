#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
static long g_q = 0; static int g_rm = 0; struct En { virtual long Query() { return g_q; } unsigned char p[0x78]; unsigned char s1[0x18]; unsigned char s2[0x18]; }; struct Ow { unsigned char p[0xcc]; En* e; }; static long Remove(Ow* o) { ++g_rm; return 7; }
static long F(Ow* o) { Lk a(o->e->s1); Lk b(o->e->s2); long r = o->e->Query(); if (r >= 0) r = Remove(o); return r; }
static bool Check() { En e; Ow o; o.e = &e; g_q = -3; if (F(&o) != -3 || g_rm != 0) return false; g_q = 1; return F(&o) == 7 && g_rm == 1 && g_enter == 4 && g_leave == 4; }

int main()
{
    if (!Check()) { std::printf("00ca50d0_TEST FAIL\n"); return 1; }
    std::printf("00ca50d0_TEST PASS\n");
    return 0;
}
