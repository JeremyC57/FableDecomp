#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
static int g_open = 0, g_apply = 0; struct En { virtual long Finish() { return g_apply == 1 && g_leave == 1 ? 5 : -1; } virtual void Apply() { if (g_enter == 2 && g_open == 1) ++g_apply; } unsigned char p[0x78]; unsigned char s1[0x18]; unsigned char s2[0x18]; }; struct Ow { unsigned char p[0xcc]; En* e; }; static void Open(Ow* o) { ++g_open; }
static long F(Ow* o) { Lk a(o->e->s1); { Lk b(o->e->s2); Open(o); o->e->Apply(); } return o->e->Finish(); }
static bool Check() { En e; Ow o; o.e = &e; return F(&o) == 5 && g_enter == 2 && g_leave == 2; }

int main()
{
    if (!Check()) { std::printf("00ca5030_TEST FAIL\n"); return 1; }
    std::printf("01216968_TEST PASS\n");
    return 0;
}