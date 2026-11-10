// test for 0x005B9CDA CUnprovenIterator_005b9cda::operator++()
#include <stdio.h>
struct CUnprovenNode_005b9cda { CUnprovenNode_005b9cda* Next; CUnprovenNode_005b9cda* Prev; };
class CUnprovenIterator_005b9cda {
public:
    CUnprovenIterator_005b9cda& operator++();
    CUnprovenNode_005b9cda* Node;
};

CUnprovenIterator_005b9cda& CUnprovenIterator_005b9cda::operator++()
{
    Node = Node->Next;
    return *this;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenNode_005b9cda a, b; a.Next = &b; b.Next = &a; CUnprovenIterator_005b9cda it; it.Node = &a;
    if (&++it != &it || it.Node != &b) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9cda_TEST FAIL\n"); return 1; }
    printf("005b9cda_TEST PASS\n");
    return 0;
}