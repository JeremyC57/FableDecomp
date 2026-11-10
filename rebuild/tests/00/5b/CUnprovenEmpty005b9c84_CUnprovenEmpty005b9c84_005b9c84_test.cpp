// test for 0x005B9C84 CUnprovenEmpty_005b9c84::CUnprovenEmpty_005b9c84()
#include <stdio.h>
class CUnprovenEmpty_005b9c84 {
public:
    CUnprovenEmpty_005b9c84();
};

CUnprovenEmpty_005b9c84::CUnprovenEmpty_005b9c84()
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenEmpty_005b9c84 e; (void)e;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c84_TEST FAIL\n"); return 1; }
    printf("005b9c84_TEST PASS\n");
    return 0;
}