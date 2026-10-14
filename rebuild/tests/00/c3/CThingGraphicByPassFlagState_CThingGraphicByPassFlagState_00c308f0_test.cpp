#include <cstdio>
static int g_n = 0; struct N { unsigned char b[16]; N(long) { ++g_n; } ~N() {} }; struct R { virtual void v() {} N* p; R(N* n) : p(n) {} ~R() {} }; struct X : R { N n; unsigned char q[0x18]; bool e; X() : R(&n), n(0), e(true) {} };
static bool Check() { X x; return g_n == 1 && x.p == &x.n && x.e; }

int main()
{
    if (!Check()) { std::printf("00c308f0_TEST FAIL\n"); return 1; }
    std::printf("00c308f0_TEST PASS\n");
    return 0;
}