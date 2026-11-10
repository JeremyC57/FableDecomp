// test for 0x005B9D47 CUnprovenIterator_005b9d47::operator->() const
#include <stdio.h>
struct CUnprovenValue_005b9d47 { long First; long Second; };
struct CUnprovenNode_005b9d47 { CUnprovenNode_005b9d47* Links[4]; CUnprovenValue_005b9d47 Value; };
class CUnprovenIterator_005b9d47 {
public:
    CUnprovenValue_005b9d47* operator->() const;
    CUnprovenNode_005b9d47* Node;
};

CUnprovenValue_005b9d47* CUnprovenIterator_005b9d47::operator->() const
{
    return &Node->Value;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenNode_005b9d47 node; node.Value.First = 7; CUnprovenIterator_005b9d47 it; it.Node = &node;
    if (it->First != 7 || it.operator->() != &node.Value) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9d47_TEST FAIL\n"); return 1; }
    printf("005b9d47_TEST PASS\n");
    return 0;
}