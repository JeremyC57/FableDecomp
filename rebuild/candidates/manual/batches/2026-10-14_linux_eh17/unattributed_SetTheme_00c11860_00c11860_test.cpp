#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Th { long v[21]; }; static int g_u = 0; struct Vw { void U() { ++g_u; } };
struct X { unsigned char p[0x7d8]; Vw vw; unsigned char q[0x4f]; bool ch; unsigned char r[3]; Th th; bool Set(const Th* t) { L l(&g_g); th = *t; ch = true; vw.U(); return true; } };
static bool Check() { Th t; for (int i = 0; i < 21; ++i) t.v[i] = i * 3; X* x = new X; x->ch = false; bool ok = x->Set(&t) && x->ch && x->th.v[20] == 60 && x->th.v[0] == 0 && g_u == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c11860_TEST FAIL\n"); return 1; }
    std::printf("00c11860_TEST PASS\n");
    return 0;
}
