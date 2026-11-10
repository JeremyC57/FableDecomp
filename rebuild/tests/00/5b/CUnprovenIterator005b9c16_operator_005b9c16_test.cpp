// test for 0x005B9C16 CUnprovenIterator_005b9c16::operator==(const CUnprovenIterator_005b9c16& other) const
#include <stdio.h>
class CUnprovenIterator_005b9c16 {
public:
    bool operator==(const CUnprovenIterator_005b9c16& other) const;
    void* Node;
};

bool CUnprovenIterator_005b9c16::operator==(const CUnprovenIterator_005b9c16& other) const
{
    return other.Node == Node;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenIterator_005b9c16 x, y, z; x.Node = &g_cell; y.Node = &g_cell; z.Node = 0;
    if (!(x == y) || (x == z)) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c16_TEST FAIL\n"); return 1; }
    printf("005b9c16_TEST PASS\n");
    return 0;
}