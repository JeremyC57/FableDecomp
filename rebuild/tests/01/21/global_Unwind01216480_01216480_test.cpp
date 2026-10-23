#include <cstdio>
static int g_b = 0; struct B { virtual void v() {} ~B() { ++g_b; } }; struct X : B { unsigned char p[0x10]; void* k; ~X() { if (k) { ::operator delete(k); k = 0; } } };
static bool Check() { { X x; x.k = ::operator new(4); } { X y; y.k = 0; } return g_b == 2; }

int main()
{
    if (!Check()) { std::printf("00c62bb0_TEST FAIL\n"); return 1; }
    std::printf("01216480_TEST PASS\n");
    return 0;
}