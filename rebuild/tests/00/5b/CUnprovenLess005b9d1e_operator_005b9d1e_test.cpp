// test for 0x005B9D1E CUnprovenLess_005b9d1e::operator()(const long& left, const long& right) const
#include <stdio.h>
class CUnprovenLess_005b9d1e {
public:
    bool operator()(const long& left, const long& right) const;
};

bool CUnprovenLess_005b9d1e::operator()(const long& left, const long& right) const
{
    return left < right;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenLess_005b9d1e lt; long a = -3, b = 2;
    if (!lt(a, b) || lt(b, a) || lt(a, a)) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9d1e_TEST FAIL\n"); return 1; }
    printf("005b9d1e_TEST PASS\n");
    return 0;
}