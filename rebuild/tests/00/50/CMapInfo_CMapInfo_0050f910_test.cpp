// behaviour test for 0x0050F910: destruction runs the member destructors once each, last declared first
#include <cstdio>
static int g_order[4]; static int g_n = 0;
struct M { int id; ~M() { g_order[g_n++] = id; } };
struct R { M a, b, c; R() { a.id = 1; b.id = 2; c.id = 3; } };
int main()
{
    { R r; }
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) { std::printf("0050f910_TEST FAIL\n"); return 1; }
    std::printf("0050f910_TEST PASS\n");
    return 0;
}