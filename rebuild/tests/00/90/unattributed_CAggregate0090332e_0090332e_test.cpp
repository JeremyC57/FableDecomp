#include <cstdio>
static int g_d = 0; struct S { void* p; ~S() { g_d = g_d * 10 + (int)(long)p; } }; struct F { void* q; ~F() { g_d = g_d * 10 + 9; } };
struct X { F a; S b, c; ~X(); }; X::~X() {}
int main()
{
    { X x; x.b.p = (void*)2; x.c.p = (void*)3; } if (g_d != 329) { std::printf("0090332e_TEST FAIL\n"); return 1; }

    std::printf("0090332e_TEST PASS\n");
    return 0;
}