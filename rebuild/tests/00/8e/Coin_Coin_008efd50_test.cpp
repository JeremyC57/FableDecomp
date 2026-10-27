// behaviour test for 0x008EFD50: the member is destroyed before the base part
#include <cstdio>
static int g_order[2]; static int g_n = 0;
struct M { ~M() { g_order[g_n++] = 1; } };
struct B { ~B() { g_order[g_n++] = 2; } };
struct D : B { M m; };
int main()
{
    { D d; }
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("008efd50_TEST FAIL\n"); return 1; }
    std::printf("008efd50_TEST PASS\n");
    return 0;
}