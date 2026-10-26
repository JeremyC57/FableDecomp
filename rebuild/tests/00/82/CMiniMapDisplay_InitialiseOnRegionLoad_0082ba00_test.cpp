#include <cstdio>
#include <string.h>
class CCharString { public:
    const char* p;
    CCharString(const char* text, long) : p(text) { }
    ~CCharString() { }
};
struct SGraphicNode { CCharString Key; long Value; SGraphicNode(const char* k, long v) : Key(k, -1), Value(v) { } };
static SGraphicNode g_nodes[4] = { SGraphicNode("", 0), SGraphicNode("MINIMAP_PRISONCOURTYARD1", 11), SGraphicNode("MINIMAP_EXECUTIONTREE", 22), SGraphicNode("MINIMAP_OAKVALE", 33) };
struct CCharStringLongMap {
    SGraphicNode* _M_header;
    SGraphicNode* find(const CCharString& key) { for (int i = 1; i < 4; ++i) if (!strcmp(g_nodes[i].Key.p, key.p)) return &g_nodes[i]; return _M_header; }
    SGraphicNode* end() const { return _M_header; }
};
class CPlayerGuiDef { public: CCharStringLongMap MiniMapGraphics; };
static CPlayerGuiDef g_def;
CPlayerGuiDef* PeekGuiDef() { return &g_def; }
class CWorldMap { public: int x; };
static CWorldMap g_map;
class CWorld { public: CWorldMap* PeekWorldMap() { return &g_map; } };
static bool g_persistent = false;
class CGame { public: bool IsLevelPersistent() { return g_persistent; } CWorld* World; };
static CWorld g_world; static CGame g_game = { &g_world };
static CGame* g_pGame = &g_game;
class CRegion { public: CCharString MiniMapGraphic; };
static int g_init = 0; static CWorldMap* g_sizeMap = 0; static CRegion* g_sizeRegion = 0;
struct CRegionMinimap { void Initialise() { ++g_init; } void InitialiseRegionMinimapSize(CWorldMap* m, CRegion* r) { g_sizeMap = m; g_sizeRegion = r; } };
class CMiniMapDisplay { public:
    long MiniMapGraphic;
    bool MiniMapIsPrisonCourtyard1, MiniMapIsPrisonCourtyard2, MiniMapIsHeadsmansHillNorth, MiniMapIsHeadsmansHillSouth;
    CRegionMinimap RegionMinimap; CRegion* PRegion; bool UpdateRequired;
    void InitialiseOnRegionLoad(CRegion* region);
};
void CMiniMapDisplay::InitialiseOnRegionLoad(CRegion* region)
{
    if (g_pGame->IsLevelPersistent())
        return;
    CPlayerGuiDef* def = PeekGuiDef();
    MiniMapIsPrisonCourtyard1 = false;
    MiniMapIsPrisonCourtyard2 = false;
    MiniMapIsHeadsmansHillNorth = false;
    MiniMapIsHeadsmansHillSouth = false;
    CCharStringLongMap& graphics = def->MiniMapGraphics;
    SGraphicNode* found = graphics.find(region->MiniMapGraphic);
    if (found != graphics.end()) {
        MiniMapGraphic = found->Value;
        SGraphicNode* courtyard;
        {
            CCharString name("MINIMAP_PRISONCOURTYARD1", -1);
            courtyard = graphics.find(name);
        }
        SGraphicNode* executionTree;
        {
            CCharString name("MINIMAP_EXECUTIONTREE", -1);
            executionTree = graphics.find(name);
        }
        if (MiniMapGraphic == courtyard->Value)
            MiniMapIsPrisonCourtyard1 = true;
        else if (MiniMapGraphic == executionTree->Value)
            MiniMapIsHeadsmansHillNorth = true;
    }
    CWorldMap* map = g_pGame->World->PeekWorldMap();
    RegionMinimap.Initialise();
    RegionMinimap.InitialiseRegionMinimapSize(map, region);
    PRegion = region;
    UpdateRequired = true;
}
static int fail() { std::printf("0082ba00_TEST FAIL\n"); return 1; }
int main()
{
    g_def.MiniMapGraphics._M_header = &g_nodes[0];
    CMiniMapDisplay d; d.MiniMapGraphic = -1; d.MiniMapIsPrisonCourtyard1 = d.MiniMapIsPrisonCourtyard2 = d.MiniMapIsHeadsmansHillNorth = d.MiniMapIsHeadsmansHillSouth = true;
    d.PRegion = 0; d.UpdateRequired = false;
    CRegion r1 = { CCharString("MINIMAP_EXECUTIONTREE", -1) };
    d.InitialiseOnRegionLoad(&r1);
    if (d.MiniMapGraphic != 22 || d.MiniMapIsPrisonCourtyard1 || !d.MiniMapIsHeadsmansHillNorth || d.MiniMapIsPrisonCourtyard2 || d.MiniMapIsHeadsmansHillSouth) return fail();
    if (g_init != 1 || g_sizeMap != &g_map || g_sizeRegion != &r1 || d.PRegion != &r1 || !d.UpdateRequired) return fail();
    CRegion r2 = { CCharString("MINIMAP_PRISONCOURTYARD1", -1) };
    d.InitialiseOnRegionLoad(&r2);
    if (d.MiniMapGraphic != 11 || !d.MiniMapIsPrisonCourtyard1 || d.MiniMapIsHeadsmansHillNorth) return fail();
    CRegion r3 = { CCharString("NOT_THERE", -1) };
    d.InitialiseOnRegionLoad(&r3);
    if (d.MiniMapGraphic != 11 || d.MiniMapIsPrisonCourtyard1 || d.PRegion != &r3 || g_init != 3) return fail();
    g_persistent = true; d.PRegion = 0; d.InitialiseOnRegionLoad(&r1);
    if (d.PRegion != 0 || g_init != 3) return fail();
    std::printf("0082ba00_TEST PASS\n");
    return 0;
}