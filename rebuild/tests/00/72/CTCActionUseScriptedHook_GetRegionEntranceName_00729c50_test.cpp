#include <cstdio>
#include <cstring>

class C3DVector { public: float X, Y, Z; };
class CCharString { public: const char* m_text; };
class CThing { public: C3DVector m_pos; const C3DVector& GetPosition() const { return m_pos; } };
struct CIntelligentPointer { CThing* m_pItem; CThing* operator->() const { return m_pItem; } };
class CRegion { public: CCharString Name; };
static int g_mapFetches = 0;
class CWorldMap { public:
    CRegion m_regions[3];
    long GetMapNumberAtPosition(const C3DVector& p) const { return p.X > 100.0f ? 12 : 4; }
    long GetRegionNumberMapIsIn(long map) const { return map >= 10 ? 2 : 1; }
    const CRegion& GetRegion(long region) const { return m_regions[region]; }
};
static CWorldMap g_worldMap;
class CTCBase { public: const CWorldMap& PeekWorldMap() const { ++g_mapFetches; return g_worldMap; } };
class CTCActionUseScriptedHook : public CTCBase { public:
    CIntelligentPointer PEntranceConnectedTo;
    const CCharString& GetRegionEntranceName() const
    {
        const CWorldMap& worldMap = PeekWorldMap();
        long mapNumber = worldMap.GetMapNumberAtPosition(PEntranceConnectedTo->GetPosition());
        return PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(mapNumber)).Name;
    }
};
int main()
{
    g_worldMap.m_regions[0].Name.m_text = "none"; g_worldMap.m_regions[1].Name.m_text = "Oakvale"; g_worldMap.m_regions[2].Name.m_text = "Bowerstone";
    CThing door; door.m_pos.X = 150.0f; door.m_pos.Y = 0.0f; door.m_pos.Z = 0.0f;
    CTCActionUseScriptedHook hook; hook.PEntranceConnectedTo.m_pItem = &door;
    const CCharString& a = hook.GetRegionEntranceName();
    if (&a != &g_worldMap.m_regions[2].Name || std::strcmp(a.m_text, "Bowerstone") != 0 || g_mapFetches != 3) { std::printf("00729c50_TEST FAIL\n"); return 1; }
    door.m_pos.X = 5.0f;
    if (std::strcmp(hook.GetRegionEntranceName().m_text, "Oakvale") != 0) { std::printf("00729c50_TEST FAIL\n"); return 1; }
    std::printf("00729c50_TEST PASS\n");
    return 0;
}