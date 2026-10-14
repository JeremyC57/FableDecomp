#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static void* g_w = 0; static short* g_a = 0; static short* g_b = 0; static void __fastcall Rd(void* w, bool f, short* s, short* t) { g_w = w; g_a = s; g_b = t; *s = 1; *t = 2; } struct X { unsigned char p[4]; short f; short s; void G(void* w, bool fl) { L l(&g_g); Rd(w, fl, &s, &f); } };
static bool Check() { X x; int i; x.G(&i, true); return g_w == &i && x.f == 2 && x.s == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c2c600_TEST FAIL\n"); return 1; }
    std::printf("00c2c600_TEST PASS\n");
    return 0;
}