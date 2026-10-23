#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_n = 3, g_p = 0; struct Src { virtual void* Next() { return g_n-- > 0 ? (void*)&g_n : 0; } };
struct X { unsigned char p[4]; Src s; void Pr(void* i) { if (i) ++g_p; } void All() { L l(&g_g); for (void* i = s.Next(); i; i = s.Next()) Pr(i); } };
static bool Check() { X x; x.All(); return g_p == 3 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c08a00_TEST FAIL\n"); return 1; }
    std::printf("01210a24_TEST PASS\n");
    return 0;
}