// test for 0x005B9C26 CUnprovenEmpty_005b9c26::~CUnprovenEmpty_005b9c26()
#include <stdio.h>
class CUnprovenEmpty_005b9c26 {
public:
    ~CUnprovenEmpty_005b9c26();
};

CUnprovenEmpty_005b9c26::~CUnprovenEmpty_005b9c26()
{
}
static long g_cell, g_cell2;
static int run()
{
    { CUnprovenEmpty_005b9c26 e; }
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c26_TEST FAIL\n"); return 1; }
    printf("005b9c26_TEST PASS\n");
    return 0;
}