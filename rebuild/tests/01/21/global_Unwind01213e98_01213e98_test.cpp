#include <cstdio>
static int g_i = 0, g_n = 0; struct N { unsigned char b[16]; N(long) { ++g_n; } ~N() {} };
struct R { virtual void v() {} N* p; R(N* n) : p(n) {} ~R() {} };
struct C : R { N n; virtual void I() { ++g_i; } __forceinline void Is() { I(); } C() : R(&n), n(1) {} ~C() {} };
struct __declspec(novtable) T : C { T() { Is(); } };
int main()
{
    T t; if (g_i != 1 || g_n != 1 || t.p != &t.n) { std::printf("00c304f0_TEST FAIL\n"); return 1; }

    std::printf("01213e98_TEST PASS\n");
    return 0;
}