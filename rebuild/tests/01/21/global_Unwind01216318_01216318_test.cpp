#include <cstdio>
static int g_r = 0, g_d = 0; struct CS; static void __stdcall Del(CS*) { ++g_d; } struct B { virtual void v() {} ~B() {} }; struct H { virtual void v() {} unsigned char s[4]; ~H() { Del((CS*)s); } }; struct X : B { virtual void R() { ++g_r; } __forceinline void D() { R(); } unsigned char p[0x18]; H h; ~X() { D(); } };
static bool Check() { { X x; } return g_r == 1 && g_d == 1; }

int main()
{
    if (!Check()) { std::printf("00c5d790_TEST FAIL\n"); return 1; }
    std::printf("01216318_TEST PASS\n");
    return 0;
}