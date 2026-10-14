#include <cstdio>
static int g_f = 0, g_s = 0, g_b = 0; struct F { unsigned char b[0x1c]; ~F() { ++g_f; } }; struct Sd { unsigned char b[4]; ~Sd() { ++g_s; } }; struct P { F f; Sd s; }; struct B { unsigned char b[0x14]; ~B() { ++g_b; } }; struct X : B { P v; ~X() {} };
static bool Check() { { X x; } return g_f == 1 && g_s == 1 && g_b == 1; }

int main()
{
    if (!Check()) { std::printf("00c1b2a0_TEST FAIL\n"); return 1; }
    std::printf("00c1b2a0_TEST PASS\n");
    return 0;
}