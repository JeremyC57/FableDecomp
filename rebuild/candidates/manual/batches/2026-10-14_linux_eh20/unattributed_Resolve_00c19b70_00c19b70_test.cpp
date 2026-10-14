#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct En {}; static En g_e; struct X { En* Find(void* k) { return k ? &g_e : 0; } long Res(En* e) { return 42; } long R(void* k) { L l(&g_g); En* e = Find(k); if (!e) return -1; return Res(e); } };
static bool Check() { X x; int a; return x.R(0) == -1 && x.R(&a) == 42 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c19b70_TEST FAIL\n"); return 1; }
    std::printf("00c19b70_TEST PASS\n");
    return 0;
}
