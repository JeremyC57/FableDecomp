#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCMapwho;
struct CThingMapManager {
    int destroyed, placed; C3DVector last;
    void DestroyMapwhoNode(CTCMapwho* m);
    void PlaceThing(CTCMapwho* m, const C3DVector& p);
};
struct CWorldMap { CThingMapManager* m_pThingMapManager; };
static CWorldMap* g_map;
struct CTCMapwho {
    unsigned char InMapwho; bool m_staysIn;
    CWorldMap* GetWorldMap() { return g_map; }
    bool IsInMapwho() const { return (InMapwho & 1) != 0; }
    void PlaceInMapwho(const C3DVector& pos)
    {
        if (!IsInMapwho()) { CWorldMap* map = GetWorldMap(); map->m_pThingMapManager->PlaceThing(this, pos); }
    }
    void MoveInMapwho(const C3DVector& pos)
    {
        if (IsInMapwho()) { CWorldMap* map = GetWorldMap(); map->m_pThingMapManager->DestroyMapwhoNode(this); }
        PlaceInMapwho(pos);
    }
};
void CThingMapManager::DestroyMapwhoNode(CTCMapwho* m) { ++destroyed; if (!m->m_staysIn) m->InMapwho &= 0xfe; }
void CThingMapManager::PlaceThing(CTCMapwho* m, const C3DVector& p) { ++placed; last = p; m->InMapwho |= 1; }
int main()
{
    CThingMapManager mgr; mgr.destroyed = mgr.placed = 0; CWorldMap map; map.m_pThingMapManager = &mgr; g_map = &map;
    CTCMapwho m; m.InMapwho = 0; m.m_staysIn = false; C3DVector p = { 1, 2, 3 };
    m.MoveInMapwho(p);
    if (mgr.destroyed != 0 || mgr.placed != 1 || mgr.last.y != 2 || !m.IsInMapwho()) { std::printf("0071a750_TEST FAIL\n"); return 1; }
    p.y = 7; m.MoveInMapwho(p);
    if (mgr.destroyed != 1 || mgr.placed != 2 || mgr.last.y != 7) { std::printf("0071a750_TEST FAIL\n"); return 1; }
    m.m_staysIn = true; m.MoveInMapwho(p);
    if (mgr.destroyed != 2 || mgr.placed != 2) { std::printf("0071a750_TEST FAIL\n"); return 1; }
    std::printf("0071a750_TEST PASS\n");
    return 0;
}