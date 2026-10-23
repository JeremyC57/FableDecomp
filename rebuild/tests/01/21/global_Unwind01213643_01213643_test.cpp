#include <cstdio>
#include <new>
static int g_node = 0; struct Nd { unsigned char b[0x28]; ~Nd() { ++g_node; } }; struct Rt { virtual void v0() {} Nd* pn; ~Rt() {} }; struct Ca : Rt { Nd n; ~Ca() {} }; struct Cb : Rt { Nd n; ~Cb() {} }; struct Cc : Rt { Nd n; ~Cc() {} }; struct Cd : Rt { Nd n; ~Cd() {} };
struct X { long m0, m4; Ca a; Cb b; Cc c; Cd d; ~X() {} };
static bool Check() { X* x = new X; delete x; return g_node == 4; }

int main()
{
    if (!Check()) { std::printf("00c2a9b0_TEST FAIL\n"); return 1; }
    std::printf("01213643_TEST PASS\n");
    return 0;
}