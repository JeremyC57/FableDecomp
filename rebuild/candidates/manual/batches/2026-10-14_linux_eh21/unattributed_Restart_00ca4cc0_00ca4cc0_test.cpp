#include <cstdio>
#include <new>
static int g_enter = 0, g_leave = 0; static void Enter(void* s) { ++g_enter; } static void Leave(void* s) { ++g_leave; } struct Lk { void* p; Lk(void* s) : p(s) { Enter(p); } ~Lk() { Leave(p); } };
static int g_stop = 0; static long g_cmd = 0; static void* g_rec = 0; struct Src { void Stop() { ++g_stop; } };
struct X { unsigned char p[0xc]; unsigned char rec[0x44]; Src* src; unsigned char q[0x10]; void* qd; unsigned char r[0xc]; long st; unsigned char s[0x1c]; unsigned char sec[0x18]; unsigned char t[0x14]; void* h; long Issue(long c, void* tg, void* rc) { g_cmd = c; g_rec = rc; return 9; } long Restart() { Lk l(sec); if (!qd) return 0; h = 0; if (src) src->Stop(); st = 1; return Issue(1, 0, rec); } };
static bool Check() { Src s; X* x = new X; int a; x->qd = 0; x->src = 0; x->h = &a; x->st = 0; if (x->Restart() != 0 || x->h != &a) return false; x->qd = &a; if (x->Restart() != 9 || x->h != 0 || g_stop) return false; x->src = &s; x->h = &a; bool ok = x->Restart() == 9 && g_stop == 1 && g_cmd == 1 && g_rec == x->rec && x->h == 0 && x->st == 1 && g_enter == 3 && g_leave == 3; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00ca4cc0_TEST FAIL\n"); return 1; }
    std::printf("00ca4cc0_TEST PASS\n");
    return 0;
}
