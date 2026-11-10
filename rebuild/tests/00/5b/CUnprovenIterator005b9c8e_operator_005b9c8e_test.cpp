// test for 0x005B9C8E CUnprovenIterator_005b9c8e::operator->() const
#include <stdio.h>
struct CUnprovenValue_005b9c8e { long First; long Second; };
struct CUnprovenNode_005b9c8e { CUnprovenNode_005b9c8e* Links[4]; CUnprovenValue_005b9c8e Value; };
class CUnprovenIterator_005b9c8e {
public:
    CUnprovenValue_005b9c8e* operator->() const;
    CUnprovenNode_005b9c8e* Node;
};

CUnprovenValue_005b9c8e* CUnprovenIterator_005b9c8e::operator->() const
{
    return &Node->Value;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenNode_005b9c8e node; node.Value.First = 7; CUnprovenIterator_005b9c8e it; it.Node = &node;
    if (it->First != 7 || it.operator->() != &node.Value) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c8e_TEST FAIL\n"); return 1; }
    printf("005b9c8e_TEST PASS\n");
    return 0;
}