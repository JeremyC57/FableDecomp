#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X { unsigned char p[0x1c]; void** e; unsigned long n; unsigned long Index() { return n - 1; } void* Pop() { L l(this); if (n == 0) return 0; void* r = e[Index()]; --n; return r; } };
static bool Check() { int a, b; void* e[2] = { &a, &b }; X x; x.e = e; x.n = 2; return x.Pop() == &b && x.Pop() == &a && x.Pop() == 0 && x.n == 0 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c4a0e0_TEST FAIL\n"); return 1; }
    std::printf("00c4a0e0_TEST PASS\n");
    return 0;
}