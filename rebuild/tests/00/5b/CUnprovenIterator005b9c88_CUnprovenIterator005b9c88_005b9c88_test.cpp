// test for 0x005B9C88 CUnprovenIterator_005b9c88::CUnprovenIterator_005b9c88()
#include <stdio.h>
class CUnprovenIterator_005b9c88 {
public:
    CUnprovenIterator_005b9c88();
    void* Node;
};

CUnprovenIterator_005b9c88::CUnprovenIterator_005b9c88() : Node(0)
{
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenIterator_005b9c88 x; if (x.Node != 0) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c88_TEST FAIL\n"); return 1; }
    printf("005b9c88_TEST PASS\n");
    return 0;
}