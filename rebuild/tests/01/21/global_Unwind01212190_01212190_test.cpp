#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Tg { virtual long Q() { return 77; } }; struct Nd { unsigned char p[0x10]; Tg t; }; static Nd g_n;
struct X { Nd* F(void* k) { return &g_n; } long Q(void* k) { L l(&g_g); return F(k)->t.Q(); } };
static bool Check() { X x; return x.Q(0) == 77 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c19cb0_TEST FAIL\n"); return 1; }
    std::printf("01212190_TEST PASS\n");
    return 0;
}