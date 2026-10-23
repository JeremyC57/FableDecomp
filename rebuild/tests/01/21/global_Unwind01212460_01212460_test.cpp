#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_f = 0, g_r = 0, g_c = 0; static G g_g2; extern "C" void Fl() { ++g_f; } struct Dv { void Cl() { ++g_c; } }; struct Ow { unsigned char p[4]; Dv* d; void R() { ++g_r; } };
struct X { unsigned char p[4]; Ow* o; void Sh() { { L a(&g_g2); Fl(); } L b(&g_g); o->R(); o->d->Cl(); } };
static bool Check() { Dv d; Ow o; o.d = &d; X x; x.o = &o; x.Sh(); return g_f == 1 && g_r == 1 && g_c == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c1c110_TEST FAIL\n"); return 1; }
    std::printf("01212460_TEST PASS\n");
    return 0;
}