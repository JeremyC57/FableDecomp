#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_s = 0; struct Tg { void S() { ++g_s; } }; struct X { unsigned char p[8]; Tg* t; void U() { L l(&g_g); t->S(); } };
static bool Check() { Tg t; X x; x.t = &t; x.U(); return g_s == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c15c70_TEST FAIL\n"); return 1; }
    std::printf("00c15c70_TEST PASS\n");
    return 0;
}