#include <cstdio>
static int g_n = 0; struct Nd { unsigned char b[0x28]; Nd(long k) { ++g_n; } ~Nd() {} }; struct Rt { virtual void v0() {} Nd* pn; Rt(Nd* n) : pn(n) {} ~Rt() {} };
struct CA : Rt { Nd n; CA() : Rt(&n), n(0) {} ~CA() {} }; struct CB : Rt { Nd n; CB() : Rt(&n), n(0) {} ~CB() {} };
struct X { void* o; CA a; CB b; X(void* O) : o(O) {} };
static bool Check() { int q; X x(&q); return x.o == &q && g_n == 2 && x.a.pn == &x.a.n && x.b.pn == &x.b.n; }

int main()
{
    if (!Check()) { std::printf("00c2bd90_TEST FAIL\n"); return 1; }
    std::printf("00c2bd90_TEST PASS\n");
    return 0;
}
