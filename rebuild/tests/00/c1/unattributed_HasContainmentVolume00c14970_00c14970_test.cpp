#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Tg { bool H(void* v) { return v != 0; } }; struct X { unsigned char p[8]; Tg* t; bool H(void* v) { L l(&g_g); return t->H(v); } };
static bool Check() { Tg t; X x; x.t = &t; int i; return x.H(&i) && !x.H(0) && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c14970_TEST FAIL\n"); return 1; }
    std::printf("00c14970_TEST PASS\n");
    return 0;
}