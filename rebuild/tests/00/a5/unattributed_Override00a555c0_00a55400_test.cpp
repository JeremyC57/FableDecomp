#include <cstdio>
static int g_c = 0; struct A { virtual void f() {} }; struct B { virtual void f() = 0; }; struct C : A, B { virtual void f() { ++g_c; } };
int main()
{
    C c; B* b = &c; b->f(); if (g_c != 1) { std::printf("00a55400_TEST FAIL\n"); return 1; }

    std::printf("00a55400_TEST PASS\n");
    return 0;
}