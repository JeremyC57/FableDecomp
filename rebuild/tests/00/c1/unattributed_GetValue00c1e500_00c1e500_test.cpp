#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Tg { float G() { return 2.5f; } }; struct O { unsigned char p[8]; Tg t; }; struct X { unsigned char p[4]; O* o; float G() { L l(&g_g); return o->t.G(); } };
static bool Check() { O o; X x; x.o = &o; return x.G() == 2.5f && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c1e500_TEST FAIL\n"); return 1; }
    std::printf("00c1e500_TEST PASS\n");
    return 0;
}