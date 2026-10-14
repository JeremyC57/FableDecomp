#include <cstdio>
static int g_next = 3, g_rem = 0, g_map = 0; struct It { }; struct CB { virtual void v0() {} virtual void Remove(It* i) { ++g_rem; } virtual It* Next() { return g_next-- > 0 ? new It : 0; } long c; ~CB() {} }; struct Mp { unsigned char b[0x28]; ~Mp() { ++g_map; } }; struct Ct : CB { Mp m; ~Ct() {} };
struct Tc : Ct { ~Tc() { It* i = Next(); while (i) { Remove(i); delete i; i = Next(); } } };
static bool Check() { { Tc t; } return g_rem == 3 && g_map == 1; }

int main()
{
    if (!Check()) { std::printf("00c3b1d0_TEST FAIL\n"); return 1; }
    std::printf("00c3b1d0_TEST PASS\n");
    return 0;
}
