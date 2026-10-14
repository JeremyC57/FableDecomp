#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CWorld; struct CMap; struct CGameMapInit { long m_a; long m_b; }; static long g_deleted = 0; static void* g_lastDeleted = 0;
struct CGameMap { CWorld* m_world; CMap* m_map; CGameMapInit m_init; CGameMap(CWorld* w, CMap* m, CGameMapInit* i) : m_world(w), m_map(m), m_init(*i) { } virtual ~CGameMap() { ++g_deleted; g_lastDeleted = this; } };
struct CMap { unsigned char _pad[0x20]; CWorld* m_pWorld; unsigned char _pad24[0x1a54]; CGameMap* m_pGameMap; unsigned char _pad1a7c[0x2a0]; CGameMapInit m_gameMapInit;
    void InitGameMap() { CGameMapInit init = m_gameMapInit; CGameMap* map = new CGameMap(m_pWorld, this, &init); CGameMap* old = m_pGameMap; m_pGameMap = map; if (old) delete old; } };

int main()
{
    CMap* map = new CMap; map->m_pWorld = (CWorld*)&map; map->m_pGameMap = 0; map->m_gameMapInit.m_a = 11; map->m_gameMapInit.m_b = 22;
    map->InitGameMap(); CGameMap* first = map->m_pGameMap; if (!first || first->m_world != (CWorld*)&map || first->m_map != map || first->m_init.m_a != 11 || first->m_init.m_b != 22 || g_deleted != 0) { std::printf("0081ee00_TEST FAIL\n"); return 1; }
    map->InitGameMap(); if (map->m_pGameMap == first || g_deleted != 1 || g_lastDeleted != first) { std::printf("0081ee00_TEST FAIL\n"); return 1; } delete map->m_pGameMap; delete map;
    std::printf("0081ee00_TEST PASS\n");
    return 0;
}
