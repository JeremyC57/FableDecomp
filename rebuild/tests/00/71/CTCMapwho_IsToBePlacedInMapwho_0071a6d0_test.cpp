#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCMapwho;
struct CThingMapManager {
    bool m_overlap; CTCMapwho* m_last;
    bool IsOverlappingCells(CTCMapwho* m, const C3DVector&) { m_last = m; return m_overlap; }
};
struct CWorldMap {
    CThingMapManager* m_pThingMapManager; float m_size;
    bool IsPosInMap(const C3DVector& p) { return p.x >= 0 && p.x < m_size && p.y >= 0 && p.y < m_size; }
};
static CWorldMap* g_map;
struct CTCMapwho {
    CWorldMap* GetWorldMap() { return g_map; }
    bool IsToBePlacedInMapwho(const C3DVector& pos)
    {
        if (GetWorldMap()->IsPosInMap(pos)) return true;
        CThingMapManager* manager = GetWorldMap()->m_pThingMapManager;
        if (!manager->IsOverlappingCells(this, pos)) return false;
        return true;
    }
};
int main()
{
    CThingMapManager mgr; mgr.m_overlap = false; mgr.m_last = 0;
    CWorldMap map; map.m_pThingMapManager = &mgr; map.m_size = 100.0f; g_map = &map;
    CTCMapwho m; C3DVector in = { 5, 5, 0 }, out = { -5, 5, 0 };
    if (!m.IsToBePlacedInMapwho(in) || mgr.m_last != 0) { std::printf("0071a6d0_TEST FAIL\n"); return 1; }
    if (m.IsToBePlacedInMapwho(out) || mgr.m_last != &m) { std::printf("0071a6d0_TEST FAIL\n"); return 1; }
    mgr.m_overlap = true;
    if (!m.IsToBePlacedInMapwho(out)) { std::printf("0071a6d0_TEST FAIL\n"); return 1; }
    std::printf("0071a6d0_TEST PASS\n");
    return 0;
}