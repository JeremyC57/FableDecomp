#include <cstdio>
static int g_f = 0, g_s = 0; struct F { unsigned char b[0x10]; ~F() { ++g_f; } }; struct Sd { unsigned char b[4]; ~Sd() { ++g_s; } }; struct X { unsigned char p[4]; F f; void* k; unsigned char q[0x1c]; Sd s; ~X() { ::operator delete(k); } };
static bool Check() { { X x; x.k = ::operator new(4); } return g_f == 1 && g_s == 1; }

int main()
{
    if (!Check()) { std::printf("00ca7c80_TEST FAIL\n"); return 1; }
    std::printf("01216c28_TEST PASS\n");
    return 0;
}