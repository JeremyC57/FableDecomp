#include <cstdio>
static int g_n = 0; struct N { unsigned char b[16]; N(long) { ++g_n; } ~N() {} }; struct R { virtual void v() {} N* p; R(N* n) : p(n) {} ~R() {} }; struct X : R { N n; X() : R(&n), n(0) {} };
static bool Check() { X x; return g_n == 1 && x.p == &x.n; }

int main()
{
    if (!Check()) { std::printf("00c3abf0_TEST FAIL\n"); return 1; }
    std::printf("01214d70_TEST PASS\n");
    return 0;
}