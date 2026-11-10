// test for 0x005B9C32 CUnprovenPair_005b9c32::GetFirst() const
#include <stdio.h>
class CUnprovenPair_005b9c32 {
public:
    void* GetFirst() const;
    void* First;
    void* Second;
};

void* CUnprovenPair_005b9c32::GetFirst() const
{
    return First;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenPair_005b9c32 p; p.First = &g_cell; p.Second = &g_cell2;
    if (p.GetFirst() != &g_cell) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c32_TEST FAIL\n"); return 1; }
    printf("005b9c32_TEST PASS\n");
    return 0;
}