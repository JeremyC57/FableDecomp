#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_r = 0; struct X { unsigned char p[0x74]; void* c; void R() { ++g_r; } void F() { L l(&g_g); if (!c) R(); } };
static bool Check() { X x; x.c = 0; x.F(); x.c = &x; x.F(); return g_r == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c09270_TEST FAIL\n"); return 1; }
    std::printf("00c09270_TEST PASS\n");
    return 0;
}