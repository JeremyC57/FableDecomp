#include <cstdio>
static int g_g = 0, g_oc = 0, g_i = -1; struct Gr { unsigned char b[0x18]; Gr() { ++g_g; } ~Gr() {} }; struct Oc { unsigned char b[0x40]; Oc() { ++g_oc; } }; struct Li { virtual void v0() {} unsigned char b[0x24]; Li() {} };
struct X { virtual void v0() {} unsigned char p[0x14]; Gr g; long f, s; Oc oc; Li li; long a0, a4, a8; short ac; void Init(long m) { g_i = (int)m; } X() : g(), f(0), s(0), oc(), li(), a0(0), a4(0), a8(0), ac(0) { Init(0); } };
static bool Check() { X x; return g_g == 1 && g_oc == 1 && g_i == 0 && x.f == 0 && x.s == 0 && x.a0 == 0 && x.ac == 0; }

int main()
{
    if (!Check()) { std::printf("00c21ba0_TEST FAIL\n"); return 1; }
    std::printf("01212a58_TEST PASS\n");
    return 0;
}