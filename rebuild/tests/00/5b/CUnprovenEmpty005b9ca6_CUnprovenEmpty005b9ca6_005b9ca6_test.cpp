// test for 0x005B9CA6 CUnprovenEmpty_005b9ca6::~CUnprovenEmpty_005b9ca6()
#include <stdio.h>
class CUnprovenEmpty_005b9ca6 {
public:
    ~CUnprovenEmpty_005b9ca6();
};

CUnprovenEmpty_005b9ca6::~CUnprovenEmpty_005b9ca6()
{
}
static long g_cell, g_cell2;
static int run()
{
    { CUnprovenEmpty_005b9ca6 e; }
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9ca6_TEST FAIL\n"); return 1; }
    printf("005b9ca6_TEST PASS\n");
    return 0;
}