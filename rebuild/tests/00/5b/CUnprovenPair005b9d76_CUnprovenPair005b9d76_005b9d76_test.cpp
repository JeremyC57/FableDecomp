// test for 0x005B9D76 CUnprovenPair_005b9d76::CUnprovenPair_005b9d76(const long& first, const bool& second)
#include <stdio.h>
class CUnprovenPair_005b9d76 {
public:
    CUnprovenPair_005b9d76(const long& first, const bool& second);
    long First;
    bool Second;
};

CUnprovenPair_005b9d76::CUnprovenPair_005b9d76(const long& first, const bool& second) : First(first), Second(second)
{
}
static long g_cell, g_cell2;
static int run()
{
    long a = 5; bool b = true; CUnprovenPair_005b9d76 p(a, b);
    if (p.First != 5 || p.Second != true) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9d76_TEST FAIL\n"); return 1; }
    printf("005b9d76_TEST PASS\n");
    return 0;
}