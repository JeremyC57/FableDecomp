#include <cstdio>
static int g_d = 0; struct S { void* p; ~S() { g_d = g_d * 10 + (int)(long)p; } };
struct X { S a, b, c; ~X() {} };
int main()
{
    { X x; x.a.p = (void*)1; x.b.p = (void*)2; x.c.p = (void*)3; } if (g_d != 321) { std::printf("00566940_TEST FAIL\n"); return 1; }

    std::printf("00566940_TEST PASS\n");
    return 0;
}