#include <cstdio>
static int g_b = 0, g_e = 0; struct B { virtual void v() {} unsigned char b[0xc30]; B() { ++g_b; } ~B() {} }; struct E { unsigned char b[0x17c]; E() { ++g_e; } ~E() {} }; struct X : B { E e[6]; X() {} };
static bool Check() { X* x = new X; bool ok = g_b == 1 && g_e == 6; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c3efa0_TEST FAIL\n"); return 1; }
    std::printf("01215180_TEST PASS\n");
    return 0;
}