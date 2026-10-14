#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Tg { virtual void a() {} virtual void b() {} virtual long Q() { return 7; } }; struct X { unsigned char p[0x18]; Tg* t; long Q() { L l(this); return t->Q(); } };
static bool Check() { Tg t; X x; x.t = &t; return x.Q() == 7 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c39af0_TEST FAIL\n"); return 1; }
    std::printf("00c39af0_TEST PASS\n");
    return 0;
}