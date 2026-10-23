#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static float g_set = 0; struct Tg { void S(float a) { g_set = a; } }; struct X { unsigned char p[8]; Tg* t; float G() { return 2.5f; } void U() { L l(&g_g); float a = G(); t->S(a); } };
static bool Check() { Tg t; X x; x.t = &t; x.U(); return g_set == 2.5f && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c15b90_TEST FAIL\n"); return 1; }
    std::printf("01211bf4_TEST PASS\n");
    return 0;
}