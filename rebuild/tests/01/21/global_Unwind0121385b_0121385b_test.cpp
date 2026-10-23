#include <cstdio>
static int g_next = 3, g_rem = 0, g_map = 0; struct It { }; struct CB { virtual void v0() {} virtual void Remove(It* i) { ++g_rem; } virtual It* Next() { return g_next-- > 0 ? new It : 0; } long c; ~CB() {} }; struct Mp { unsigned char b[0x28]; ~Mp() { ++g_map; } }; struct Ct : CB { Mp m; ~Ct() {} };
struct X { unsigned char p[4]; Ct a; Ct b; ~X() {} };
static bool Check() { { X x; } return g_map == 2; }

int main()
{
    if (!Check()) { std::printf("00c2be10_TEST FAIL\n"); return 1; }
    std::printf("0121385b_TEST PASS\n");
    return 0;
}