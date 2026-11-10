// test for 0x005B9C0B CUnprovenIterator_005b9c0b::CUnprovenIterator_005b9c0b(CUnprovenNode_005b9c0b* node)
#include <stdio.h>
struct CUnprovenNode_005b9c0b;
class CUnprovenIterator_005b9c0b {
public:
    CUnprovenIterator_005b9c0b(CUnprovenNode_005b9c0b* node);
    CUnprovenNode_005b9c0b* Node;
};

CUnprovenIterator_005b9c0b::CUnprovenIterator_005b9c0b(CUnprovenNode_005b9c0b* node) : Node(node)
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenNode_005b9c0b* n = (CUnprovenNode_005b9c0b*)&g_cell;
    CUnprovenIterator_005b9c0b it(n);
    if (it.Node != n) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c0b_TEST FAIL\n"); return 1; }
    printf("005b9c0b_TEST PASS\n");
    return 0;
}