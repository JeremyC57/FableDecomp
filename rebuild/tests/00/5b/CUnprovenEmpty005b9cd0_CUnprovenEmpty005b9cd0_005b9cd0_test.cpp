// test for 0x005B9CD0 CUnprovenEmpty_005b9cd0::CUnprovenEmpty_005b9cd0()
#include <stdio.h>
class CUnprovenEmpty_005b9cd0 {
public:
    CUnprovenEmpty_005b9cd0();
};

CUnprovenEmpty_005b9cd0::CUnprovenEmpty_005b9cd0()
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenEmpty_005b9cd0 e; (void)e;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9cd0_TEST FAIL\n"); return 1; }
    printf("005b9cd0_TEST PASS\n");
    return 0;
}