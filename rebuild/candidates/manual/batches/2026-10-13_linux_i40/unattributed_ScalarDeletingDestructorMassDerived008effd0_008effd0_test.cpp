#include <cstdio>
static int g_d = 0; struct L { void* h; ~L() { ++g_d; } }; struct B { virtual ~B() { g_d += 10; } }; struct X : B { L l; };
static void Kill(X* x) { delete x; }
int main()
{
    Kill(new X); if (g_d != 11) { std::printf("008effd0_TEST FAIL\n"); return 1; }

    std::printf("008effd0_TEST PASS\n");
    return 0;
}
