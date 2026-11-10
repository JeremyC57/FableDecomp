// test for 0x005B9C42 CUnprovenIterator_005b9c42::CUnprovenIterator_005b9c42(const CUnprovenIterator_005b9c42& other)
#include <stdio.h>
class CUnprovenIterator_005b9c42 {
public:
    CUnprovenIterator_005b9c42() {}
    CUnprovenIterator_005b9c42(const CUnprovenIterator_005b9c42& other);
    void* Node;
};

CUnprovenIterator_005b9c42::CUnprovenIterator_005b9c42(const CUnprovenIterator_005b9c42& other) : Node(other.Node)
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenIterator_005b9c42 x; x.Node = &g_cell;
    CUnprovenIterator_005b9c42 y(x);
    if (y.Node != &g_cell) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c42_TEST FAIL\n"); return 1; }
    printf("005b9c42_TEST PASS\n");
    return 0;
}