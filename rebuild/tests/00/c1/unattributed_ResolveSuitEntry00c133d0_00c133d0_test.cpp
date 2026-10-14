#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct En {}; static En g_e; static void* g_arg = 0; struct Mp { En* Find(void* k) { return k ? &g_e : 0; } }; struct Ow { unsigned char p[0x68]; Mp* m; };
struct X { unsigned char p[4]; Ow* o; long Res(En* e, void* a) { g_arg = a; return 9; } long R(void* k, void* a) { L l(&g_g); En* e = o->m->Find(k); if (!e) return -1; return Res(e, a); } };
static bool Check() { Mp m; Ow o; o.m = &m; X x; x.o = &o; int a, b; return x.R(0, &b) == -1 && x.R(&a, &b) == 9 && g_arg == &b && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c133d0_TEST FAIL\n"); return 1; }
    std::printf("00c133d0_TEST PASS\n");
    return 0;
}