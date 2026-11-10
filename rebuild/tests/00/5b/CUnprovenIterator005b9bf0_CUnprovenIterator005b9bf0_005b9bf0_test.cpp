// test for 0x005B9BF0 CUnprovenIterator_005b9bf0::CUnprovenIterator_005b9bf0(CUnprovenNode_005b9bf0* node)
#include <stdio.h>
struct CUnprovenNode_005b9bf0;
class CUnprovenIterator_005b9bf0 {
public:
    CUnprovenIterator_005b9bf0(CUnprovenNode_005b9bf0* node);
    CUnprovenNode_005b9bf0* Node;
};

CUnprovenIterator_005b9bf0::CUnprovenIterator_005b9bf0(CUnprovenNode_005b9bf0* node) : Node(node)
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenNode_005b9bf0* n = (CUnprovenNode_005b9bf0*)&g_cell;
    CUnprovenIterator_005b9bf0 it(n);
    if (it.Node != n) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9bf0_TEST FAIL\n"); return 1; }
    printf("005b9bf0_TEST PASS\n");
    return 0;
}