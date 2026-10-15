// Behaviour test for 0x007E92D0 CTCAvoidRegionExit::RebuildStaticThingLists: both static lists are cleared first, then the
// first is filled through a HasTC(99) + InMap(world map, current map) filter and the second through a leaf-type(100) filter.
#include <cstdio>
struct CThingList { int m_cleared; int m_filled; int m_type; const void* m_world; long m_map; void clear() { ++m_cleared; m_filled = 0; } };
struct CWorldMap { long m_current; long GetCurrentMap() const { return m_current; } };
struct CThingFilter_HasTC { long m_type; CThingFilter_HasTC(long t) : m_type(t) {} };
struct CThingFilter_InMap { const CWorldMap* m_worldMap; long m_map; CThingFilter_InMap(const CWorldMap& w, long m) : m_worldMap(&w), m_map(m) {} };
struct CFilter_Bind2 {
    const CThingFilter_HasTC* m_first; const CThingFilter_InMap* m_second;
    CFilter_Bind2(const CThingFilter_HasTC& a, const CThingFilter_InMap& b) : m_first(&a), m_second(&b) {}
};
struct CThingFilter_HasTCOfLeafType { long m_type; CThingFilter_HasTCOfLeafType(long t) : m_type(t) {} };
struct CThingSearchTools {
    void GetAllThings(const CFilter_Bind2& f, CThingList& out) { out.m_filled = 1; out.m_type = f.m_first->m_type; out.m_world = f.m_second->m_worldMap; out.m_map = f.m_second->m_map; }
    void GetAllThings(const CThingFilter_HasTCOfLeafType& f, CThingList& out) { out.m_filled = 1; out.m_type = f.m_type; }
};
static CThingList g_A, g_B;
static CWorldMap g_world;
static CThingSearchTools g_tools;
struct CTCAvoidRegionExit {
    const CWorldMap& PeekWorldMap() const { return g_world; }
    CThingSearchTools* GetThingSearchTools() const { return &g_tools; }
    void RebuildStaticThingLists()
    {
        g_A.clear();
        g_B.clear();
        GetThingSearchTools()->GetAllThings(CFilter_Bind2(CThingFilter_HasTC(99), CThingFilter_InMap(PeekWorldMap(), PeekWorldMap().GetCurrentMap())), g_A);
        GetThingSearchTools()->GetAllThings(CThingFilter_HasTCOfLeafType(100), g_B);
    }
};
int main()
{
    g_world.m_current = 17;
    CTCAvoidRegionExit tc;
    tc.RebuildStaticThingLists();
    if (g_A.m_cleared != 1 || g_B.m_cleared != 1 || !g_A.m_filled || g_A.m_type != 99 || g_A.m_world != &g_world || g_A.m_map != 17
        || !g_B.m_filled || g_B.m_type != 100) { std::printf("007e92d0_TEST FAIL\n"); return 1; }
    std::printf("007e92d0_TEST PASS\n");
    return 0;
}
