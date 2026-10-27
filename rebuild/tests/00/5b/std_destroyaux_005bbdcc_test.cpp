// behaviour test for 0x005BBDCC: every element in the range is destroyed exactly once
#include <cstdio>
static int g_dtors = 0;
struct E { long a; long b; long c; ~E() { ++g_dtors; } };
static void DestroyRange(E* f, E* l) { for (; f != l; ++f) f->~E(); }
int main()
{
    E v[4]; DestroyRange(v, v + 4); DestroyRange(v, v);
    int ok = g_dtors == 4; g_dtors = 0;
    if (!ok) { std::printf("005bbdcc_TEST FAIL\n"); return 1; }
    std::printf("005bbdcc_TEST PASS\n");
    return 0;
}