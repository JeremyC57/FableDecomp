#include <cstdio>
static int g_n = 0; struct Nd { unsigned char b[8]; Nd(long k) { g_n += (int)k; } ~Nd() {} }; struct Rt { virtual void v0() {} Nd* pn; Rt(Nd* n) : pn(n) {} ~Rt() {} };
struct CA : Rt { Nd n; CA() : Rt(&n), n(1) {} ~CA() {} }; struct CB : Rt { Nd n; CB() : Rt(&n), n(1) {} ~CB() {} };
struct X { CA a; CB b; X() {} };
static bool Check() { X x; return g_n == 2 && x.a.pn == &x.a.n && x.b.pn == &x.b.n; }

int main()
{
    if (!Check()) { std::printf("00c0a6d0_TEST FAIL\n"); return 1; }
    std::printf("01210dc0_TEST PASS\n");
    return 0;
}