#include <cstdio>
static int g_b = 0, g_e = 0; struct CB { virtual void v() {} CB() { ++g_b; } ~CB() {} }; struct E { unsigned char b[0x28]; E() { ++g_e; } ~E() {} }; struct X : CB { E e[4]; X() {} };
static bool Check() { X x; return g_b == 1 && g_e == 4; }

int main()
{
    if (!Check()) { std::printf("00c6a280_TEST FAIL\n"); return 1; }
    std::printf("01216540_TEST PASS\n");
    return 0;
}