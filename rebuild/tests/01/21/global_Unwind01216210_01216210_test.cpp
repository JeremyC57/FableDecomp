#include <cstdio>
#include <new>
static int g_m = 0, g_i = -1; struct Rt { virtual void v0() {} ~Rt() {} }; struct Mb { unsigned char b[0x9c]; Mb() { ++g_m; } };
struct X : Rt { long m4; long m8, mc, m10, m14, m18, m1c, m20, m24, m28; unsigned char p[0xd0]; long fc; unsigned char q[4]; bool b104; unsigned char r[7]; bool b10c; unsigned char s[3]; Mb mb; long m1ac; bool b1b0; unsigned char t[3]; long m1b4; void Init(long m) { g_i = (int)m; } X() : mb() { m4 = 0; m1b4 = 0; b10c = false; Init(1); m8 = 0; mc = 0; m10 = 0; m14 = 0; m18 = 0; m1c = 0; m20 = 0; m24 = 0; m28 = 0; fc = 0; m1ac = 0; b104 = false; b1b0 = false; } };
static bool Check() { X* x = new X; bool ok = g_m == 1 && g_i == 1 && x->m4 == 0 && x->m28 == 0 && x->fc == 0 && !x->b104 && !x->b10c && !x->b1b0 && x->m1b4 == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c55cd0_TEST FAIL\n"); return 1; }
    std::printf("01216210_TEST PASS\n");
    return 0;
}