// test for 0x005B9C2E CUnprovenPair_005b9c2e::GetSecond() const
#include <stdio.h>
class CUnprovenPair_005b9c2e {
public:
    void* GetSecond() const;
    void* First;
    void* Second;
};

void* CUnprovenPair_005b9c2e::GetSecond() const
{
    return Second;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenPair_005b9c2e p; p.First = &g_cell; p.Second = &g_cell2;
    if (p.GetSecond() != &g_cell2) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c2e_TEST FAIL\n"); return 1; }
    printf("005b9c2e_TEST PASS\n");
    return 0;
}