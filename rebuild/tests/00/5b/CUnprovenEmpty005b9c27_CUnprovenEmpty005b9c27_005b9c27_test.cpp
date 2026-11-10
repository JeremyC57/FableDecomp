// test for 0x005B9C27 CUnprovenEmpty_005b9c27::CUnprovenEmpty_005b9c27()
#include <stdio.h>
class CUnprovenEmpty_005b9c27 {
public:
    CUnprovenEmpty_005b9c27();
};

CUnprovenEmpty_005b9c27::CUnprovenEmpty_005b9c27()
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenEmpty_005b9c27 e; (void)e;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c27_TEST FAIL\n"); return 1; }
    printf("005b9c27_TEST PASS\n");
    return 0;
}