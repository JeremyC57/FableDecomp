#include <cstdio>
static void* g_a = 0; static void* g_o = 0; struct B { unsigned char b[0x14]; B(void* a) { g_a = a; } ~B() {} }; struct M { unsigned char b[4]; M(void* o, void* a) { g_o = o; } ~M() {} }; struct X : B { M m; X(void* a) : B(a), m(this, a) {} };
static bool Check() { int i; X x(&i); return g_a == &i && g_o == &x; }

int main()
{
    if (!Check()) { std::printf("00c1b6d0_TEST FAIL\n"); return 1; }
    std::printf("01212420_TEST PASS\n");
    return 0;
}