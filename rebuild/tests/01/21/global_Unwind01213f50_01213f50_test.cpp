#include <cstdio>
static int g_n = 0; struct A { unsigned char b[16]; A() { ++g_n; } ~A() {} }; struct B { unsigned char b[16]; B() { ++g_n; } ~B() {} }; struct C { unsigned char b[16]; C() { ++g_n; } ~C() {} }; struct X { A a; B b; C c; X() : a(), b(), c() {} };
static bool Check() { X x; return g_n == 3; }

int main()
{
    if (!Check()) { std::printf("00c2eed0_TEST FAIL\n"); return 1; }
    std::printf("01213f50_TEST PASS\n");
    return 0;
}