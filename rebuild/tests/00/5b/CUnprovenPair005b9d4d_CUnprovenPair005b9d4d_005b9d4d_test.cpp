// test for 0x005B9D4D CUnprovenPair_005b9d4d::CUnprovenPair_005b9d4d(const long& first, const long& second)
#include <stdio.h>
class CUnprovenPair_005b9d4d {
public:
    CUnprovenPair_005b9d4d(const long& first, const long& second);
    long First;
    long Second;
};

CUnprovenPair_005b9d4d::CUnprovenPair_005b9d4d(const long& first, const long& second) : First(first), Second(second)
{
}
static long g_cell, g_cell2;
static int run()
{
    long a = 5; long b = 9; CUnprovenPair_005b9d4d p(a, b);
    if (p.First != 5 || p.Second != 9) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9d4d_TEST FAIL\n"); return 1; }
    printf("005b9d4d_TEST PASS\n");
    return 0;
}