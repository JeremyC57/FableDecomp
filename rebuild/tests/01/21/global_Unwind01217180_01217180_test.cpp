#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
#include <cstring>
static int g_ex = 0; static long g_res = 0; static void* g_last = 0; static bool g_tag = true;
struct X { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual long Execute(void* t, long m, void* a, void* b, const char* tag) { ++g_ex; g_last = t; if (m != 0 || std::strcmp(tag, "ExecuteTask")) g_tag = false; return g_res; } unsigned char p[0x18]; unsigned char sec[0x18]; long m34; void* fa; void* fb; void* sa; void* sb; long st; long Run(void* a, void* b) { Lk l(sec); if (st == 1) return 0x80004005; long r = Execute(a, 0, fa, fb, "ExecuteTask"); if (b && r >= 0) r = Execute(b, 0, sa, sb, "ExecuteTask"); return r; } };
static bool Check() { X x; int a, b; x.st = 1; if (x.Run(&a, &b) != 0x80004005L || g_ex) return false; x.st = 0; g_res = -1; if (x.Run(&a, &b) != -1 || g_ex != 1) return false; g_res = 3; if (x.Run(&a, 0) != 3 || g_ex != 2) return false; return x.Run(&a, &b) == 3 && g_ex == 4 && g_last == &b && g_tag && g_enter == 4 && g_leave == 4; }

int main()
{
    if (!Check()) { std::printf("00cadd00_TEST FAIL\n"); return 1; }
    std::printf("01217180_TEST PASS\n");
    return 0;
}