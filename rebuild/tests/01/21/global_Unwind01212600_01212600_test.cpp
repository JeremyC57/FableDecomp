#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Sub { long C() { return 5; } }; struct Tg { unsigned char p[8]; Sub s; long F() { return 7; } long Se() { return 11; } };
struct X { unsigned char p[4]; Tg* t; long Q() { L l(&g_g); return t->s.C() + t->F() + t->Se(); } };
static bool Check() { Tg t; X x; x.t = &t; return x.Q() == 23 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c1e280_TEST FAIL\n"); return 1; }
    std::printf("01212600_TEST PASS\n");
    return 0;
}