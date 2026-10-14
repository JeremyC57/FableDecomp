#include <cstdio>
static int g_a = 0, g_b = 0; struct NA { unsigned char b[0x30]; NA() { ++g_a; } ~NA() {} }; struct NB { unsigned char b[0x30]; NB() { ++g_b; } ~NB() {} };
struct X { void* o; void* m; void* mt; NA a; NB b; long c; long cap; bool f0, f1, f2, f3, f4, f5, f6; X(void* O, void* M, void* MT) : o(O), m(M), mt(MT) { c = 0; cap = 0; f0 = f1 = f2 = f3 = f4 = f5 = f6 = false; } };
static bool Check() { int a, b, c; X x(&a, &b, &c); return x.o == &a && x.m == &b && x.mt == &c && g_a == 1 && g_b == 1 && x.c == 0 && x.cap == 0 && !x.f0 && !x.f6; }

int main()
{
    if (!Check()) { std::printf("00c2e0c0_TEST FAIL\n"); return 1; }
    std::printf("00c2e0c0_TEST PASS\n");
    return 0;
}