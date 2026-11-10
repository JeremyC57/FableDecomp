// test for 0x005B9C5E CUnprovenPair_005b9c5e::GetFirst() const
#include <stdio.h>
class CUnprovenPair_005b9c5e {
public:
    void* GetFirst() const;
    void* First;
    void* Second;
};

void* CUnprovenPair_005b9c5e::GetFirst() const
{
    return First;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenPair_005b9c5e p; p.First = &g_cell; p.Second = &g_cell2;
    if (p.GetFirst() != &g_cell) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c5e_TEST FAIL\n"); return 1; }
    printf("005b9c5e_TEST PASS\n");
    return 0;
}