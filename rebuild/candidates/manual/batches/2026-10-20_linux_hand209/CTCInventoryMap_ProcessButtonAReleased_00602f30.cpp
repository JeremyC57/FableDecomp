// CTCInventoryMap::ProcessButtonAReleased() -- A released on the map screen: in selection state 1 with the world map active and a region chosen, leaves the world map (drops the map primitives, starts the mini-map file loading for the region, rebuilds the quest and teleport markers and re-selects); in state 0 with teleporting allowed and the screen not opened from the hero-information screens, teleports to the active region; otherwise the base handler
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00602F30, length 208 bytes (+ int3 padding); speed TU (no frame pointer).
// Callees: CTCInventoryMap's map-primitive release @ 0x005FC510 (manifest "ComboBox_ClearAllLists" is propagated; it clears the three
// MapScreen*PrimitiveHandles vectors), CTCBase::GetWorld() const @ 0x00686A80 (manifest "CViewBase::GetDisplayEngine"), CWorld virtual slot
// 13 (+0x34, PeekWorldMap), CWorldMap::GetRegion(long) @ 0x004FC180 (CRegion&), CPlayerGui::GetPMiniMapDisplay() @ 0x00437CE0 on g_pPlayerGui
// (0x013B8790), CMiniMapDisplay::InitialiseMiniMapFileLoading_Region(CRegion*, CCharString*, bool) @ 0x00829D90 with CWorld::CurrentWorldName
// (+0x84), CTCInventoryMap::FindQuestsForActiveRegion @ 0x005FD0F0 and FindTeleportsForActiveRegion @ 0x005FD1D0, CTCInventoryBase virtual
// slot 46 (+0xB8, the selection hook taking 0), the NHeroInformationScreens::CBase secondary base's virtual slot 3 (+0x130),
// CTCInventoryMap::TeleportToActiveRegion @ 0x005FBFA0 and CTCInventoryBase::ProcessButtonAReleased @ 0x005BC65C (both tail-jumped).
// Layout: CTCInventoryBase InventorySelectionState +0x1C; retail WorldMapActive +0x234 and ActiveRegionIndex +0x238 (PDB +0x290/+0x294);
// CWorld teleport-allowed byte +0xD5 (named from use, as the landed IsTeleportValid 0x005FAA60); the secondary base at +0x130.
// Shape: the region reference is a named local (edi) kept across the second GetWorld call (a named world local whose name member is
// pushed by address); the player GUI is read into a local right after that call, before the argument pushes; the two `else` arms
// end in tail jumps.
struct CCharString { void* PStringData; };
struct CRegion;
struct CWorldMap {
    CRegion& GetRegion(long index);                // 0x004FC180
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();              // slot 12
    virtual CWorldMap* PeekWorldMap();             // slot 13 (+0x34)
    unsigned char _pad04[0x84 - 0x04];
    CCharString   CurrentWorldName;                // +0x84
    unsigned char _pad88[0xd5 - 0x88];
    unsigned char m_teleportAllowed;               // +0xD5 (name from use)
};
struct CMiniMapDisplay {
    void InitialiseMiniMapFileLoading_Region(CRegion* region, CCharString* worldName, bool b);   // 0x00829D90
};
struct CPlayerGui {
    CMiniMapDisplay* GetPMiniMapDisplay();         // 0x00437CE0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
namespace NHeroInformationScreens {
struct CBase {                                     // secondary base at +0x130
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool IsOpenedFromHeroInformationScreens();   // slot 3 (+0x0C)
};
}
struct CTCInventoryBase_Map {                      // retail 0x130 bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45();
    virtual void SelectItem(long index);           // slot 46 (+0xB8)
    unsigned char _pad04[0x1c - 0x04];
    long          InventorySelectionState;         // +0x1c
    unsigned char _pad20[0x130 - 0x20];
    CWorld* GetWorld() const;                      // 0x00686A80 (CTCBase)
    void ProcessButtonAReleased();                 // 0x005BC65C
};
class CTCInventoryMap : public CTCInventoryBase_Map, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad134[0x234 - 0x134];
    bool          WorldMapActive;                  // +0x234
    long          ActiveRegionIndex;               // +0x238
    void ReleaseMapScreenPrimitives();             // 0x005FC510
    void FindQuestsForActiveRegion();              // 0x005FD0F0
    void FindTeleportsForActiveRegion();           // 0x005FD1D0
    void TeleportToActiveRegion();                 // 0x005FBFA0
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
