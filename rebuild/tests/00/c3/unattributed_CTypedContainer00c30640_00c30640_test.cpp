#include <cstdio>
static int g_r = 0, g_d = 0, g_n = 0; struct I { ~I() { ++g_d; } }; struct N { unsigned char b[16]; N(long) {} ~N() { ++g_n; } };
struct R { virtual void v() {} N* p; R(N* n) : p(n) {} ~R() {} };
struct C : R { N n; long k; virtual void O(I*) { ++g_r; } virtual I* F() { return k-- > 0 ? new I : 0; }
    __forceinline void Cl() { while (I* i = F()) { O(i); delete i; } } C() : R(&n), n(1), k(2) {} ~C() {} };
struct __declspec(novtable) T : C { ~T() { Cl(); } };
int main()
{
    { T t; } if (g_r != 2 || g_d != 2 || g_n != 1) { std::printf("00c30640_TEST FAIL\n"); return 1; }

    std::printf("00c30640_TEST PASS\n");
    return 0;
}