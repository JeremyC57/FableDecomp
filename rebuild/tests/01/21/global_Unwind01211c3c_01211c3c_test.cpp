#include <cstdio>
#include <new>
static int g_lk = 0; static bool g_ok = true; static long g_h = 3, g_t = 4; struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } }; struct St { bool Query(long* h, long* e, long* t) { *h = g_h; *e = 0; *t = g_t; return g_ok; } }; static unsigned char g_sec[8];
struct X { unsigned char p[8]; St* s; long Calc(float* r) { Pl l(g_sec); long h, e, t; if (!s->Query(&h, &e, &t)) return 0; if (h < 0 || t < 0) return 1; *r = (float)h / t; return 2; } };
static bool Check() { St s; X x; x.s = &s; float r = 0; if (x.Calc(&r) != 2 || r != 0.75f) return false; g_h = -1; if (x.Calc(&r) != 1) return false; g_h = 1; g_t = -2; if (x.Calc(&r) != 1) return false; g_ok = false; return x.Calc(&r) == 0 && g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c15d50_TEST FAIL\n"); return 1; }
    std::printf("00c15d50_TEST PASS\n");
    return 0;
}