#include <cstdio>
static int g_a = 0; static void* g_f = 0; struct F { unsigned char b[0x14]; F(long k) { g_a = k; } ~F() {} }; struct Sd { unsigned char b[4]; Sd(F* f, long) { g_f = f; } ~Sd() {} }; struct X { virtual void v() {} F f; Sd s; X() : f(8), s(&f, 8) {} };
static bool Check() { X x; return g_a == 8 && g_f == &x.f; }

int main()
{
    if (!Check()) { std::printf("00c485e0_TEST FAIL\n"); return 1; }
    std::printf("00c485e0_TEST PASS\n");
    return 0;
}