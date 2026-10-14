#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_step = 0, g_u1 = 0, g_u2 = 0; struct A { void U() { ++g_u1; } }; struct B { void U() { ++g_u2; } };
struct X { unsigned char p[8]; A* a; B* b; unsigned char q[0x60]; long fr; void St() { ++g_step; } void Adv() { L l(&g_g); St(); a->U(); b->U(); ++fr; } };
static bool Check() { A a; B b; X x; x.a = &a; x.b = &b; x.fr = 5; x.Adv(); return g_step == 1 && g_u1 == 1 && g_u2 == 1 && x.fr == 6 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c14dd0_TEST FAIL\n"); return 1; }
    std::printf("00c14dd0_TEST PASS\n");
    return 0;
}
