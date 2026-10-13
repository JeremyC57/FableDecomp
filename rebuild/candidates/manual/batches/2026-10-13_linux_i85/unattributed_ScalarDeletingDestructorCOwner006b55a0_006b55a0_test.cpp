#include <cstdio>
static int g_d = 0; struct S { void* p; ~S() { ++g_d; } };
struct X { long a, b; S s; };
static void Kill(X* x) { delete x; }
int main()
{
    Kill(new X); if (g_d != 1) { std::printf("006b55a0_TEST FAIL\n"); return 1; }

    std::printf("006b55a0_TEST PASS\n");
    return 0;
}
