#include <cstdio>
static int g_m = 0; struct B { virtual void v00() {} B() {} ~B() {} }; struct M { void* d; M() : d(0) { ++g_m; } ~M() {} };
struct X : B { void* p; M m; virtual void v00() {} X(void* v) : B(), p(v), m() {} };
int main()
{
    int q; X x(&q); if (x.p != &q || g_m != 1) { std::printf("00c1fb00_TEST FAIL\n"); return 1; }

    std::printf("012127a0_TEST PASS\n");
    return 0;
}