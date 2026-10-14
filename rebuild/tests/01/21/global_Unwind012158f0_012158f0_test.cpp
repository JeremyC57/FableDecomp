#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_rel = 0; static void* g_pushed = 0; static long g_pend = 0; struct Mb { virtual void v0() {} virtual void Push(void* m) { g_pushed = m; } virtual void v2() {} virtual long HasPending() { return g_pend; } }; struct Sm { void Release() { ++g_rel; } };
struct X { unsigned char p[0x18]; Mb* mb; Sm sm; long P(void* m) { L l(this); if (mb->HasPending()) return 1; sm.Release(); mb->Push(m); return 0; } };
static bool Check() { Mb mb; X x; x.mb = &mb; int a; g_pend = 1; if (x.P(&a) != 1 || g_rel) return false; g_pend = 0; return x.P(&a) == 0 && g_rel == 1 && g_pushed == &a && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c48c80_TEST FAIL\n"); return 1; }
    std::printf("00c48c80_TEST PASS\n");
    return 0;
}