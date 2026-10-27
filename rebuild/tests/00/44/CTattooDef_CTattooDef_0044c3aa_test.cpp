// behaviour test for 0x0044C3AA CTattooDef::CTattooDef() (model: each class-type member is constructed once, in declaration order)
#include <cstdio>
static int g_order[8]; static int g_n = 0;
struct M { M(int id) { g_order[g_n++] = id; } };
struct Def { M a, b, c; Def() : a(1), b(2), c(3) { } };
int main()
{
    Def d; (void)d;
    int ok = g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3;
    if (!ok) { std::printf("0044c3aa_TEST FAIL\n"); return 1; }
    std::printf("0044c3aa_TEST PASS\n");
    return 0;
}