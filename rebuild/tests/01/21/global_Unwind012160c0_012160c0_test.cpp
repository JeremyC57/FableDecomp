#include <cstdio>
#include <cstdlib>
static int g_c = 0; struct P { unsigned char b[0x4f0]; static void* operator new(unsigned int n) { return std::malloc(n); } static void operator delete(void* p) { std::free(p); } P(long) { ++g_c; } }; struct X { unsigned char q[0xe4]; P* p; long C() { p = new P(1); return 0; } };
static bool Check() { X* x = new X; bool ok = x->C() == 0 && x->p && g_c == 1; delete x->p; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c4e2f0_TEST FAIL\n"); return 1; }
    std::printf("012160c0_TEST PASS\n");
    return 0;
}