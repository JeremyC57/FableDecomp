// test for 0x005B9CD3 CUnprovenEmpty_005b9cd3::~CUnprovenEmpty_005b9cd3()
#include <stdio.h>
class CUnprovenEmpty_005b9cd3 {
public:
    ~CUnprovenEmpty_005b9cd3();
};

CUnprovenEmpty_005b9cd3::~CUnprovenEmpty_005b9cd3()
{
}
static long g_cell, g_cell2;
static int run()
{
    { CUnprovenEmpty_005b9cd3 e; }
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9cd3_TEST FAIL\n"); return 1; }
    printf("005b9cd3_TEST PASS\n");
    return 0;
}