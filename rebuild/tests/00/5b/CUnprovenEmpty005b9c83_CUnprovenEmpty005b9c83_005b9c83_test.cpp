// test for 0x005B9C83 CUnprovenEmpty_005b9c83::~CUnprovenEmpty_005b9c83()
#include <stdio.h>
class CUnprovenEmpty_005b9c83 {
public:
    ~CUnprovenEmpty_005b9c83();
};

CUnprovenEmpty_005b9c83::~CUnprovenEmpty_005b9c83()
{
}
static long g_cell, g_cell2;
static int run()
{
    { CUnprovenEmpty_005b9c83 e; }
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c83_TEST FAIL\n"); return 1; }
    printf("005b9c83_TEST PASS\n");
    return 0;
}