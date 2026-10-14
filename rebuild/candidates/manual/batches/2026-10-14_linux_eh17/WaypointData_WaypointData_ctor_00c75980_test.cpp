#include <cstdio>
static int g_base = 0, g_s = 0, g_b = 0; struct Bs { virtual void v0() {} Bs() { ++g_base; } ~Bs() {} }; struct Sm { unsigned char b[0x14]; Sm() { ++g_s; } ~Sm() {} }; struct Lg { unsigned char b[0x28]; Lg() { ++g_b; } ~Lg() {} };
struct X : Bs { Sm s[3]; Lg l[2]; long c; X() : c(0) {} };
static bool Check() { X x; return g_base == 1 && g_s == 3 && g_b == 2 && x.c == 0; }

int main()
{
    if (!Check()) { std::printf("00c75980_TEST FAIL\n"); return 1; }
    std::printf("00c75980_TEST PASS\n");
    return 0;
}
