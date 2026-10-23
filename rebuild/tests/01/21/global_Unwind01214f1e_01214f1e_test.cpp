#include <cstdio>
static int g_s = 0, g_c = 0; struct Sl { unsigned char b[0x14]; ~Sl() { ++g_s; } }; struct Bs { virtual void v0() {} unsigned char b[0x18]; ~Bs() {} };
struct X : Bs { Sl s1, s2, s3, s4; void Cl() { ++g_c; } ~X() { Cl(); } };
static bool Check() { { X x; } return g_s == 4 && g_c == 1; }

int main()
{
    if (!Check()) { std::printf("00c3cb30_TEST FAIL\n"); return 1; }
    std::printf("01214f1e_TEST PASS\n");
    return 0;
}