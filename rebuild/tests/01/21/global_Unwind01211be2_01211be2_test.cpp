#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_q = 0; struct Tg { bool Q() { ++g_q; return false; } };
struct X { unsigned char p[8]; Tg* t; bool Has() { L l(&g_g); Tg* tg = t; if (!tg) return true; return tg->Q(); } };
static bool Check() { Tg t; X x; x.t = 0; if (!x.Has()) return false; x.t = &t; return !x.Has() && g_q == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c15b00_TEST FAIL\n"); return 1; }
    std::printf("01211be2_TEST PASS\n");
    return 0;
}