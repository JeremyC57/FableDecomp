#include <cstdio>
static int g_e = 0, g_b = 0; struct CB { virtual void v() {} ~CB() { ++g_b; } }; struct E { unsigned char b[0x28]; ~E() { ++g_e; } }; struct __declspec(novtable) X : CB { E e[4]; ~X() {} };
static bool Check() { { X x; } return g_e == 4 && g_b == 1; }

int main()
{
    if (!Check()) { std::printf("00c6a2e0_TEST FAIL\n"); return 1; }
    std::printf("00c6a2e0_TEST PASS\n");
    return 0;
}