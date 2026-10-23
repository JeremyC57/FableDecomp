#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static void* g_v = 0; static unsigned short g_i = 0; struct Tg { float A(void* v, unsigned short i) { g_v = v; g_i = i; return 1.0f; } }; struct O { unsigned char p[0x34]; Tg* t; }; struct X { unsigned char p[4]; O* o; unsigned char q[6]; unsigned short i; unsigned char r[4]; void* v; void U() { L l(&g_g); o->t->A(v, i); } };
static bool Check() { Tg t; O o; o.t = &t; X x; x.o = &o; x.i = 7; int k; x.v = &k; x.U(); return g_v == &k && g_i == 7 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c15c00_TEST FAIL\n"); return 1; }
    std::printf("01211c06_TEST PASS\n");
    return 0;
}