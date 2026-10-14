#include <cstdio>
#include <cstdlib>
static int g_b = 0; static void* __cdecl Al(unsigned int n) { return std::malloc(n); } struct B { unsigned char b[0x18]; B() { ++g_b; } ~B() {} }; struct X : B { long n; void* p; long u; void* m; X(long c) { n = c; p = Al(c * 4); u = 0; m = 0; } };
static bool Check() { X x(3); bool ok = g_b == 1 && x.n == 3 && x.p && x.u == 0 && x.m == 0; std::free(x.p); return ok; }

int main()
{
    if (!Check()) { std::printf("00c49c90_TEST FAIL\n"); return 1; }
    std::printf("00c49c90_TEST PASS\n");
    return 0;
}