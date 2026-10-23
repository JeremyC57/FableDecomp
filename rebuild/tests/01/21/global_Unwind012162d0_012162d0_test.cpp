#include <cstdio>
static int g_r = 0; struct B { virtual void v() {} ~B() {} }; struct X : B { virtual void R() { ++g_r; } __forceinline void D() { R(); } ~X() { D(); } };
int main()
{
    { X x; } if (g_r != 1) { std::printf("00c577b0_TEST FAIL\n"); return 1; }
    std::printf("012162d0_TEST PASS\n");
    return 0;
}