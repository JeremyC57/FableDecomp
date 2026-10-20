// Behaviour test for CTCInventoryMap::ProcessButtonAReleased @ 0x00602F30 (model copy; stub world, GUI, base handler).
#include <cstdio>
struct CCharString { void* PStringData; };
struct CRegion { long id; };
struct CWorldMap { CRegion regions[4]; CRegion& GetRegion(long index) { return regions[index]; } };
struct CWorld {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual CWorldMap* GetWorldMap() { return 0; }
    virtual CWorldMap* PeekWorldMap() { return &map; }
    unsigned char _pad04[0x84 - 0x04]; CCharString CurrentWorldName; unsigned char _pad88[0xd5 - 0x88]; unsigned char m_teleportAllowed;
    CWorldMap map;
};
struct CMiniMapDisplay { CRegion* region; CCharString* name; bool flag; long inits; void InitialiseMiniMapFileLoading_Region(CRegion* r, CCharString* n, bool b) { ++inits; region = r; name = n; flag = b; } };
struct CPlayerGui { CMiniMapDisplay display; CMiniMapDisplay* GetPMiniMapDisplay() { return &display; } };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
namespace NHeroInformationScreens {
struct CBase { virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual bool IsOpenedFromHeroInformationScreens() = 0; };
}
struct CTCInventoryBase_Map {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {} virtual void v24() {}
    virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {} virtual void v29() {}
    virtual void v30() {} virtual void v31() {} virtual void v32() {} virtual void v33() {} virtual void v34() {}
    virtual void v35() {} virtual void v36() {} virtual void v37() {} virtual void v38() {} virtual void v39() {}
    virtual void v40() {} virtual void v41() {} virtual void v42() {} virtual void v43() {} virtual void v44() {}
    virtual void v45() {}
    virtual void SelectItem(long index) { ++selects; lastSelected = index; }
    unsigned char _pad04[0x1c - 0x04]; long InventorySelectionState; unsigned char _pad20[0x130 - 0x20];
    CWorld* world; long selects; long lastSelected; long baseReleases;
    CWorld* GetWorld() const { return world; }
    void ProcessButtonAReleased() { ++baseReleases; }
};
class CTCInventoryMap : public CTCInventoryBase_Map, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad134[0x234 - 0x134]; bool WorldMapActive; long ActiveRegionIndex;
    long releases, quests, teleports, teleportsToRegion; bool openedFromHeroInfo;
    virtual bool IsOpenedFromHeroInformationScreens() { return openedFromHeroInfo; }
    void ReleaseMapScreenPrimitives() { ++releases; }
    void FindQuestsForActiveRegion() { ++quests; }
    void FindTeleportsForActiveRegion() { ++teleports; }
    void TeleportToActiveRegion() { ++teleportsToRegion; }
    void ProcessButtonAReleased();
};
void CTCInventoryMap::ProcessButtonAReleased()
{
    if (InventorySelectionState == 1) {
        if (WorldMapActive && ActiveRegionIndex > 0) {
            WorldMapActive = false;
            ReleaseMapScreenPrimitives();
            CRegion& region = GetWorld()->PeekWorldMap()->GetRegion(ActiveRegionIndex);
            CWorld* world = GetWorld();
            CPlayerGui* gui = g_pPlayerGui;
            gui->GetPMiniMapDisplay()->InitialiseMiniMapFileLoading_Region(&region, &world->CurrentWorldName, false);
            FindQuestsForActiveRegion();
            FindTeleportsForActiveRegion();
            SelectItem(0);
            return;
        }
    } else if (InventorySelectionState == 0) {
        if (GetWorld()->m_teleportAllowed && !IsOpenedFromHeroInformationScreens()) {
            TeleportToActiveRegion();
            return;
        }
    }
    CTCInventoryBase_Map::ProcessButtonAReleased();
}
int main()
{
    CWorld world; world.m_teleportAllowed = 1; for (int i = 0; i < 4; ++i) world.map.regions[i].id = 50 + i;
    g_gui.display.inits = 0;
    CTCInventoryMap map; map.world = &world; map.selects = 0; map.baseReleases = 0; map.releases = map.quests = map.teleports = map.teleportsToRegion = 0;
    map.openedFromHeroInfo = false; map.lastSelected = -1;
    bool ok = true;
    // state 1, world map active, region 2: leave the world map
    map.InventorySelectionState = 1; map.WorldMapActive = true; map.ActiveRegionIndex = 2;
    map.ProcessButtonAReleased();
    if (map.WorldMapActive || map.releases != 1 || g_gui.display.inits != 1 || g_gui.display.region != &world.map.regions[2]) ok = false;
    if (g_gui.display.name != &world.CurrentWorldName || g_gui.display.flag || map.quests != 1 || map.teleports != 1 || map.selects != 1 || map.lastSelected != 0 || map.baseReleases != 0) ok = false;
    // state 1 but no region selected: the base handler
    map.WorldMapActive = true; map.ActiveRegionIndex = 0;
    map.ProcessButtonAReleased();
    if (!map.WorldMapActive || map.releases != 1 || map.baseReleases != 1) ok = false;
    // state 0 with teleport allowed and not opened from the hero screens: teleport
    map.InventorySelectionState = 0;
    map.ProcessButtonAReleased();
    if (map.teleportsToRegion != 1 || map.baseReleases != 1) ok = false;
    map.openedFromHeroInfo = true; map.ProcessButtonAReleased();
    if (map.teleportsToRegion != 1 || map.baseReleases != 2) ok = false;
    map.openedFromHeroInfo = false; world.m_teleportAllowed = 0; map.ProcessButtonAReleased();
    if (map.teleportsToRegion != 1 || map.baseReleases != 3) ok = false;
    // any other state: the base handler
    map.InventorySelectionState = 2; map.ProcessButtonAReleased();
    if (map.baseReleases != 4 || map.releases != 1 || map.teleportsToRegion != 1) ok = false;
    if (!ok) { std::printf("00602f30_TEST FAIL\n"); return 1; }
    std::printf("00602f30_TEST PASS\n");
    return 0;
}
