// test for 0x005B9D63 CUnprovenIterator_005b9d63::CUnprovenIterator_005b9d63(const CUnprovenIterator_005b9d63& other)
#include <stdio.h>
class CUnprovenIterator_005b9d63 {
public:
    CUnprovenIterator_005b9d63() {}
    CUnprovenIterator_005b9d63(const CUnprovenIterator_005b9d63& other);
    void* Node;
};

CUnprovenIterator_005b9d63::CUnprovenIterator_005b9d63(const CUnprovenIterator_005b9d63& other) : Node(other.Node)
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenIterator_005b9d63 x; x.Node = &g_cell;
    CUnprovenIterator_005b9d63 y(x);
    if (y.Node != &g_cell) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9d63_TEST FAIL\n"); return 1; }
    printf("005b9d63_TEST PASS\n");
    return 0;
}