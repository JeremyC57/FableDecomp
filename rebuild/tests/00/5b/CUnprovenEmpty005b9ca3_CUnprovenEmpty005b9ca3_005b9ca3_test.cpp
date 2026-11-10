// test for 0x005B9CA3 CUnprovenEmpty_005b9ca3::CUnprovenEmpty_005b9ca3()
#include <stdio.h>
class CUnprovenEmpty_005b9ca3 {
public:
    CUnprovenEmpty_005b9ca3();
};

CUnprovenEmpty_005b9ca3::CUnprovenEmpty_005b9ca3()
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenEmpty_005b9ca3 e; (void)e;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9ca3_TEST FAIL\n"); return 1; }
    printf("005b9ca3_TEST PASS\n");
    return 0;
}