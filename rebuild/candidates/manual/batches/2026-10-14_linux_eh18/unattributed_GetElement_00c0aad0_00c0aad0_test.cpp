#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X { unsigned char p[0x344]; float e[8]; float Get(unsigned long i) { L l(&g_g); if (i >= 8) return 100.0f; return e[i]; } };
static bool Check() { X* x = new X; for (int i = 0; i < 8; ++i) x->e[i] = (float)i; bool ok = x->Get(3) == 3.0f && x->Get(7) == 7.0f && x->Get(8) == 100.0f && x->Get(0xffffffff) == 100.0f && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0aad0_TEST FAIL\n"); return 1; }
    std::printf("00c0aad0_TEST PASS\n");
    return 0;
}
