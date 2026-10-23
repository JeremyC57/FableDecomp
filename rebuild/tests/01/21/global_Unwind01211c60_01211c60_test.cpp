#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_u = 0; struct X { unsigned char p[0x14]; long a; void U() { ++g_u; } void S(long v) { L l(&g_g); a = v; U(); } };
static bool Check() { X x; x.S(3); return x.a == 3 && g_u == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c15ea0_TEST FAIL\n"); return 1; }
    std::printf("01211c60_TEST PASS\n");
    return 0;
}