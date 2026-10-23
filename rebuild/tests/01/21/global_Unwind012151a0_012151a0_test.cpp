#include <cstdio>
static int g_e = 0, g_b = 0; struct B { virtual void v() {} ~B() { ++g_b; } }; struct E { unsigned char b[0x17c]; ~E() { ++g_e; } }; struct __declspec(novtable) X : B { unsigned char p[0xc30]; E e[6]; ~X() {} };
static bool Check() { { X* x = new X; delete x; } return g_e == 6 && g_b == 1; }

int main()
{
    if (!Check()) { std::printf("00c3f030_TEST FAIL\n"); return 1; }
    std::printf("012151a0_TEST PASS\n");
    return 0;
}