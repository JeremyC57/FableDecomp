// test for 0x005B9D05 CUnprovenLess_005b9d05::operator()(const long& left, const long& right) const
#include <stdio.h>
class CUnprovenLess_005b9d05 {
public:
    bool operator()(const long& left, const long& right) const;
};

bool CUnprovenLess_005b9d05::operator()(const long& left, const long& right) const
{
    return left < right;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenLess_005b9d05 lt; long a = -3, b = 2;
    if (!lt(a, b) || lt(b, a) || lt(a, a)) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9d05_TEST FAIL\n"); return 1; }
    printf("005b9d05_TEST PASS\n");
    return 0;
}