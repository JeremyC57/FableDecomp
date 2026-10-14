#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct En {}; static En g_e; static void* g_a = 0; static void* g_b = 0; struct X { En* Find(void* k) { return k ? &g_e : 0; } long Res(En* e, void* a, void* b) { g_a = a; g_b = b; return 5; } long R(void* k, void* a, void* b) { L l(&g_g); En* e = Find(k); if (!e) return 0; return Res(e, a, b); } };
static bool Check() { X x; int a, b, c; return x.R(0, &b, &c) == 0 && x.R(&a, &b, &c) == 5 && g_a == &b && g_b == &c && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c19ad0_TEST FAIL\n"); return 1; }
    std::printf("00c19ad0_TEST PASS\n");
    return 0;
}