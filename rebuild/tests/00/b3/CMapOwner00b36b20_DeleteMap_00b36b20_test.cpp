#include <cstdio>
static int g_deleted = 0;
struct CMap { virtual ~CMap() { ++g_deleted; } };
struct COwner { CMap* m_pMap; void DeleteMap() { delete m_pMap; m_pMap = 0; } };
int main()
{
    COwner o; o.m_pMap = 0; o.DeleteMap(); if (g_deleted != 0 || o.m_pMap != 0) { std::printf("00b36b20_TEST FAIL\n"); return 1; }
    o.m_pMap = new CMap; o.DeleteMap(); if (g_deleted != 1 || o.m_pMap != 0) { std::printf("00b36b20_TEST FAIL\n"); return 1; }
    std::printf("00b36b20_TEST PASS\n"); return 0;
}