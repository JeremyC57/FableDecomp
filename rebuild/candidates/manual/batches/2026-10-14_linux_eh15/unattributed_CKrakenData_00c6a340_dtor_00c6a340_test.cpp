#include <cstdio>
static int g_s = 0, g_b = 0, g_base = 0; struct Base { virtual void v0() {} ~Base() { ++g_base; } }; struct Sm { unsigned char b[0x14]; ~Sm() { ++g_s; } }; struct Lg { unsigned char b[0x28]; ~Lg() { ++g_b; } };
struct X : Base { Sm s[3]; Lg l[2]; ~X() {} };
static bool Check() { { X x; } return g_s == 3 && g_b == 2 && g_base == 1; }

int main()
{
    if (!Check()) { std::printf("00c6a340_TEST FAIL\n"); return 1; }
    std::printf("00c6a340_TEST PASS\n");
    return 0;
}
