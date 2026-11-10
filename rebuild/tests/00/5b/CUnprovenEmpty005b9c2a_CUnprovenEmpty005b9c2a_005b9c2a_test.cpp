// test for 0x005B9C2A CUnprovenEmpty_005b9c2a::~CUnprovenEmpty_005b9c2a()
#include <stdio.h>
class CUnprovenEmpty_005b9c2a {
public:
    ~CUnprovenEmpty_005b9c2a();
};

CUnprovenEmpty_005b9c2a::~CUnprovenEmpty_005b9c2a()
{
}
static long g_cell, g_cell2;
static int run()
{
    { CUnprovenEmpty_005b9c2a e; }
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c2a_TEST FAIL\n"); return 1; }
    printf("005b9c2a_TEST PASS\n");
    return 0;
}