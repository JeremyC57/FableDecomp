#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CGameMapGrid { long m_cleared; void Clear() { ++m_cleared; } };
struct CMap { unsigned char _pad[0x1a7c]; CGameMapGrid* m_pGameMapGrid; unsigned char _pad1a80[0x280]; long m_1d00; long m_1d04; long m_1d08;
    void Clear() { m_pGameMapGrid->Clear(); m_1d00 = 0; m_1d04 = 0; m_1d08 = 0; } };

int main()
{
    CGameMapGrid g; g.m_cleared = 0; CMap* map = new CMap; map->m_pGameMapGrid = &g; map->m_1d00 = 1; map->m_1d04 = 2; map->m_1d08 = 3;
    map->Clear(); if (g.m_cleared != 1 || map->m_1d00 || map->m_1d04 || map->m_1d08) { std::printf("0081e7e0_TEST FAIL\n"); return 1; } delete map;
    std::printf("0081e7e0_TEST PASS\n");
    return 0;
}