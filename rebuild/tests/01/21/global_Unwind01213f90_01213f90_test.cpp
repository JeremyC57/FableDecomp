#include <cstdio>
static int g_v = 0; struct V { virtual void v() {} V() {} ~V() {} }; struct X { virtual void A(V*) { ++g_v; } virtual void* G() { return g_v ? 0 : this; } void F(long) { if (G()) { V v; A(&v); } } };
static bool Check() { X x; x.F(0); x.F(0); return g_v == 1; }

int main()
{
    if (!Check()) { std::printf("00c308a0_TEST FAIL\n"); return 1; }
    std::printf("01213f90_TEST PASS\n");
    return 0;
}