#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_s = 0; struct X { void S() { ++g_s; } void U() { L l(&g_g); S(); } };
static bool Check() { X x; x.U(); return g_s == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c1c6a0_TEST FAIL\n"); return 1; }
    std::printf("00c1c6a0_TEST PASS\n");
    return 0;
}