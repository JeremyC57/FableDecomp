#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static void* g_n = 0; static void* g_p = 0; static void* g_c = 0; struct Tr { void S(void* n, void* p, void* c) { g_n = n; g_p = p; g_c = c; } }; struct X { unsigned char p[0xc]; Tr t; void S(void* n, void* c) { L l(&g_g); t.S(n, 0, c); } };
static bool Check() { X x; int i, j; x.S(&i, &j); return g_n == &i && g_p == 0 && g_c == &j && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c38600_TEST FAIL\n"); return 1; }
    std::printf("0121479c_TEST PASS\n");
    return 0;
}