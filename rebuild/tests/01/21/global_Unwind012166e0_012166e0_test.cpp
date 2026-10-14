#include <cstdio>
static int g_c = 0, g_d = 0; struct Sl { virtual void v() {} ~Sl() {} }; struct Li { unsigned char b[4]; void C() { ++g_c; } ~Li() { ++g_d; } }; struct X { unsigned char p[0x234]; Sl s; unsigned char q[0x54]; Li l; ~X() { l.C(); } };
static bool Check() { { X* x = new X; delete x; } return g_c == 1 && g_d == 1; }

int main()
{
    if (!Check()) { std::printf("00c6ae90_TEST FAIL\n"); return 1; }
    std::printf("00c6ae90_TEST PASS\n");
    return 0;
}