#include <cstdio>
static int g_d = 0; struct S { void* p; void* q; ~S() { g_d = g_d * 10 + 2; } }; struct B { long x; ~B() { g_d = g_d * 10 + 1; } };
struct X : B { S a, b; };
static void Kill(X* x) { delete x; }
int main()
{
    Kill(new X); if (g_d != 221) { std::printf("0084b270_TEST FAIL\n"); return 1; }

    std::printf("0084b270_TEST PASS\n");
    return 0;
}
