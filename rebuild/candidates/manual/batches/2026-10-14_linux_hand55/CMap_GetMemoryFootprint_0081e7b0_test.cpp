#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
struct CGameMapGrid { long m_size; long GetMemoryFootprint() { note("G"); return m_size; } };
struct CQuarterGrid { long m_size; long GetMemoryFootprint() { note("Q"); return m_size; } };
struct CMap { unsigned char _pad[0x1a78]; CQuarterGrid* m_pQuarterGrid; CGameMapGrid* m_pGameMapGrid;
    long GetMemoryFootprint() { long footprint = m_pGameMapGrid->GetMemoryFootprint(); return m_pQuarterGrid->GetMemoryFootprint() + footprint; } };

int main()
{
    CGameMapGrid g; g.m_size = 1000; CQuarterGrid q; q.m_size = 24; CMap* map = new CMap; map->m_pQuarterGrid = &q; map->m_pGameMapGrid = &g;
    if (map->GetMemoryFootprint() != 1024 || std::strcmp(g_seq, "GQ")) { std::printf("0081e7b0_TEST FAIL\n"); return 1; } delete map;
    std::printf("0081e7b0_TEST PASS\n");
    return 0;
}
