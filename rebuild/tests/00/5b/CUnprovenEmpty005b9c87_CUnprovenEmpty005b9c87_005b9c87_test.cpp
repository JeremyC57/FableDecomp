// test for 0x005B9C87 CUnprovenEmpty_005b9c87::~CUnprovenEmpty_005b9c87()
#include <stdio.h>
class CUnprovenEmpty_005b9c87 {
public:
    ~CUnprovenEmpty_005b9c87();
};

CUnprovenEmpty_005b9c87::~CUnprovenEmpty_005b9c87()
{
}
static long g_cell, g_cell2;
static int run()
{
    { CUnprovenEmpty_005b9c87 e; }
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c87_TEST FAIL\n"); return 1; }
    printf("005b9c87_TEST PASS\n");
    return 0;
}