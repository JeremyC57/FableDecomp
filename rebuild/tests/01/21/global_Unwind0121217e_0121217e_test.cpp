#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct En {}; static En g_e; static void* g_arg = 0; struct X { En* Find(void* k) { return k ? &g_e : 0; } long Res(En* e, void* a) { g_arg = a; return 42; } long R(void* k, void* a) { L l(&g_g); En* e = Find(k); if (!e) return -1; return Res(e, a); } };
static bool Check() { X x; int a, b; return x.R(0, &b) == -1 && x.R(&a, &b) == 42 && g_arg == &b && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c19c10_TEST FAIL\n"); return 1; }
    std::printf("0121217e_TEST PASS\n");
    return 0;
}