#include <cstdio>
static int g_d = 0; struct B { void* d; ~B() { g_d = g_d * 10 + 1; } }; struct M { void* d; ~M() { g_d = g_d * 10 + 2; } };
struct X : B { unsigned char pad[0x18]; M m; ~X() {} };
int main()
{
    { X x; } if (g_d != 21) { std::printf("00c39ba0_TEST FAIL\n"); return 1; }

    std::printf("00c39ba0_TEST PASS\n");
    return 0;
}