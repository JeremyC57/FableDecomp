// test for 0x005B9D34 CUnprovenIterator_005b9d34::operator->() const
#include <stdio.h>
struct CUnprovenValue_005b9d34 { long First; long Second; };
struct CUnprovenNode_005b9d34 { CUnprovenNode_005b9d34* Links[4]; CUnprovenValue_005b9d34 Value; };
class CUnprovenIterator_005b9d34 {
public:
    CUnprovenValue_005b9d34* operator->() const;
    CUnprovenNode_005b9d34* Node;
};

CUnprovenValue_005b9d34* CUnprovenIterator_005b9d34::operator->() const
{
    return &Node->Value;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenNode_005b9d34 node; node.Value.First = 7; CUnprovenIterator_005b9d34 it; it.Node = &node;
    if (it->First != 7 || it.operator->() != &node.Value) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9d34_TEST FAIL\n"); return 1; }
    printf("005b9d34_TEST PASS\n");
    return 0;
}