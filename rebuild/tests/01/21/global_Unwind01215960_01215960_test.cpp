#include <cstdio>
static int g_o = 0, g_b = 0; struct O { ~O() { ++g_o; } }; struct B { unsigned char b[0x38]; ~B() { ++g_b; } }; struct X : B { O* o; ~X() { delete o; o = 0; } };
static bool Check() { { X x; x.o = new O; } { X y; y.o = 0; } return g_o == 1 && g_b == 2; }

int main()
{
    if (!Check()) { std::printf("00c48290_TEST FAIL\n"); return 1; }
    std::printf("01215960_TEST PASS\n");
    return 0;
}