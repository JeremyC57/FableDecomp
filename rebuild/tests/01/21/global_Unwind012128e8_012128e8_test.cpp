#include <cstdio>
static int g_m = 0, g_s = 0; struct M { unsigned char b[0x10]; ~M() { ++g_m; } }; struct D { char* p; ~D() { ++g_s; } };
struct B { virtual ~B() {} }; struct F : B { void* m; M mm; }; struct P { F f; D s; ~P() {} };
int main()
{
    { P p; } if (g_m != 1 || g_s != 1) { std::printf("00c201d0_TEST FAIL\n"); return 1; }

    std::printf("012128e8_TEST PASS\n");
    return 0;
}