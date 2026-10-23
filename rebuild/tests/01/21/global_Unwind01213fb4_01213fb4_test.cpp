#include <cstdio>
#include <new>
static int g_next = 3, g_rem = 0, g_map = 0, g_mem = 0; struct Mb { unsigned char b[4]; ~Mb() { ++g_mem; } }; struct It { long k; Mb m; }; struct CB { virtual void v0() {} virtual void Remove(It* i) { ++g_rem; } virtual It* Next() { return g_next-- > 0 ? new It : 0; } long c; ~CB() {} }; struct Mp { unsigned char b[0x28]; ~Mp() { ++g_map; } }; struct Ct : CB { Mp m; ~Ct() {} };
struct Tc : Ct { ~Tc() { It* i = Next(); while (i) { Remove(i); delete i; i = Next(); } } };
static bool Check() { { Tc t; } return g_rem == 3 && g_mem == 3 && g_map == 1; }

int main()
{
    if (!Check()) { std::printf("00c30940_TEST FAIL\n"); return 1; }
    std::printf("01213fb4_TEST PASS\n");
    return 0;
}