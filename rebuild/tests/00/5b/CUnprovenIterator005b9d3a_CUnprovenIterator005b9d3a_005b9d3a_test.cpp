// test for 0x005B9D3A CUnprovenIterator_005b9d3a::CUnprovenIterator_005b9d3a(const CUnprovenIterator_005b9d3a& other)
#include <stdio.h>
class CUnprovenIterator_005b9d3a {
public:
    CUnprovenIterator_005b9d3a() {}
    CUnprovenIterator_005b9d3a(const CUnprovenIterator_005b9d3a& other);
    void* Node;
};

CUnprovenIterator_005b9d3a::CUnprovenIterator_005b9d3a(const CUnprovenIterator_005b9d3a& other) : Node(other.Node)
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenIterator_005b9d3a x; x.Node = &g_cell;
    CUnprovenIterator_005b9d3a y(x);
    if (y.Node != &g_cell) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9d3a_TEST FAIL\n"); return 1; }
    printf("005b9d3a_TEST PASS\n");
    return 0;
}