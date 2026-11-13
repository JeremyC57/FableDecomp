#ifndef FABLE_ENGINE_CTCINVENTORYMAP_H
#define FABLE_ENGINE_CTCINVENTORYMAP_H
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CHeroLogBook.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
class CRegion;
#if defined(FABLE_RETAIL_CTCINVENTORYMAP_TELEPORT) || defined(FABLE_RETAIL_CTCINVENTORYMAP_FRAMEUPDATE) || defined(FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS)
class CThing;
#endif
#include "engine/NHeroInformationScreens_CBase.h"
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
class C3DVector;
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_QUESTMARKERS
namespace NUISystem { class CComponent; }
#include "stlport/fable_map.h"
#endif
// Layout: original TPI2aa6d and native constructor allocation258. The selected
// serialiser independently addresses every used member at the offsets below.
class CTCInventoryMap : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_STATGETINTERFACETYPE
 // Retail opt-in (agent4007, row 0x0061B610 CTCInventoryQuests::ConstructQuestList): FableWin.pdb
 // ?StatGetInterfaceType@CTCInventoryMap@@SI?AW4ETCInterfaceType@@XZ (0x01A526E0, 5 instructions), inline in retail: the row's
 // one-argument CThing::GetTC<CTCInventoryMap>(T**) reads it (TCI_INVENTORY_MAP = 27, `push 1Bh` at the lookups). No layout change.
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_INVENTORY_MAP; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_PCMAPLIST
 // Retail opt-in (chunk 1007, row 0x00589A30 CTCInventory::OpenPCMapMenu): Ego_r.pdb / FableWin.pdb
 // ?ConstructPCMapList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@_NPA_N@Z, out of line at 0x0060EEB0 (`ret 8`; the Ego_r donor
 // order ConstructMapList 0x0065F950 / ConstructPCMapList 0x006621A0 matches retail ConstructMapList 0x0060C900, `ret 0Ch`, three
 // arguments). The bound spelling BuildMapMenu is not a PDB name (NAME_FIXES). Declaration only.
 NUISystem::CComponent* ConstructPCMapList(bool a, bool* b);          // 0x0060EEB0
#endif
 void AddLogBookSaveInfo(const CWideString&,const CWideString&,const CWideString&,CHeroLogBook::CHeroLogBookEntry::ECategory);
 class CUIMiniMapMarker {
 public: long Region;CCharString UIDefName;C2DVector Position;long DefNameLength,GraphicIndex;float Angle;
  CUIMiniMapMarker():Region(0),UIDefName(""),Position(0.0f,0.0f),DefNameLength(0),GraphicIndex(0),Angle(0.0f){}
 };
 class CUIRegionInfo {
 public:long Region,Shops,BarberShops,TitleShops,TatooShops,ShopsForSale,HousesForSale,ShopsOwned,HousesOwned,Taverns;
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_REGIONINFO_CTOR
  // Chunk 778 (row 0x00615E70): Ego_r.pdb ??0CUIRegionInfo@CTCInventoryMap@@QAE@XZ, inline in retail (every member zeroed: the
  // traits' DefaultVal in 0x00615E10 and the `value = CUIRegionInfo()` temporary in 0x00615E70).
  CUIRegionInfo():Region(0),Shops(0),BarberShops(0),TitleShops(0),TatooShops(0),ShopsForSale(0),HousesForSale(0),ShopsOwned(0),HousesOwned(0),Taverns(0){}
#endif
 };
protected:
 virtual void OnSerialise(CPersistContext&,bool);
 std::vector<long> VisitedRegions;
 float AccumFadeTime;
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK
 // Retail opt-in (chunk 883, row 0x006096E0 ConstructLogbookListPC): the real type, Ego_r.pdb CategoriesList
 // map<CHeroLogBook::CHeroLogBookEntry::ECategory,NUISystem::CComponent*> (the landed ~CTCInventoryMap 0x00606F80 erases it with
 // that tree's _M_erase 0x00613AE0; 12 bytes, same offset +0x144).
 // Retail opt-in (row 0x00607330 ConstructLogbookList): the real type of CategoriesList (Ego_r.pdb member name and type, map<ECategory,
 // CComponent*>; Ego_r's Dinkumware offset is 0x160) as the 12-byte STLport map at retail +0x144 (`add esi,144h` 0x00607506,
 // `add ecx,144h` 0x0060830E / 0x0060852D ..., `cmp ecx,[esi+144h]` 0x0060949E).
 std::map<CHeroLogBook::CHeroLogBookEntry::ECategory,NUISystem::CComponent*> CategoriesList;
#else
 unsigned char CategoriesList[12];
#endif
 std::vector<CWideString> LogBookNameTag,LogBookAbbreviatedNameTag,LogBookTextTag;
 std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory> LogBookCategory;
 long GenerateDataTimer;
 bool MiniMapIsPrisonCourtyard1,MiniMapIsPrisonCourtyard2,MiniMapIsHeadsmansHillNorth,MiniMapIsHeadsmansHillSouth;
private:
 unsigned char pmap_real[4];
 std::vector<CUIMiniMapMarker> MiniMapMarkers;
 std::vector<CUIRegionInfo> UIRegionsInfo;
 long QuestToBeDropped;
 bool FirstTimeCheckingCurrentRegion;
 unsigned char MapScreenPrimitiveHandles[12],MapScreenQuestPrimitiveHandles[12];
 unsigned char MapScreenTeleportPrimitiveHandles[12],MapScreenHousePrimitiveHandles[12];
 unsigned char ActiveRegionQuestPositions[12],ActiveRegionTeleportPositions[12],ActiveRegionHousePositions[12];
 CCountedPointer<std::vector<unsigned __int64> > SerialisedTeleporterUIDs;
 unsigned char TeleportRegions[12],MapList[4],MapRegions[12];
 C2DBoxI MapBox;
 bool WorldMapActive;
 long ActiveRegionIndex,NumRegions;
 bool InventoryScreen,OpenedViaGuildSeal;
 NHeroInformationScreens::EScreenType PreviousScreenType;
 bool PanMode;
 std::list<std::pair<const CRegion*,const CRegion*> > RegionsCorrespondances;
 long LastEventFrame;
 unsigned long RegionEventLife;
 void RemapUIDsForLoading(std::vector<unsigned __int64>&)const;
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK
 // (same opt-in) Ego_r.pdb + FableWin.pdb ?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z (public;
 // retail 0x006096E0, the PC twin of ConstructLogbookList 0x00607330 and in the same donor order) and
 // ?GetMenuEntryFromCategoryPC@CTCInventoryMap@@IAEPAVCComponent@NUISystem@@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAV23@@Z
 // (protected, landed 0x005FAC20). Declarations only.
public:
 NUISystem::CComponent* ConstructLogbookListPC(NUISystem::CComponent*);
protected:
 NUISystem::CComponent* GetMenuEntryFromCategoryPC(CHeroLogBook::CHeroLogBookEntry::ECategory,NUISystem::CComponent*);
#endif
 #ifdef FABLE_RETAIL_CTCINVENTORYMAP_TELEPORT
 // Nested opt-in (chunk 1011, rows 0x005FBEA0 / 0x005FBFA0 / 0x005FB4B0): Ego_r.pdb spellings
 // ?GetRegionTeleporter@CTCInventoryMap@@ABEPAVCThing@@ABVCRegion@@@Z (private const, 0x005FBEA0),
 // ?TeleportToActiveRegion@CTCInventoryMap@@AAEXXZ (private, 0x005FBFA0) and ?TeleportHeroToBoastingArea@CTCInventoryMap@@QBEXXZ
 // (public const, 0x005FB4B0; the landed CTCInventoryQuests::TeleportToBoastingPodium 0x0061A360 calls it). Declarations only.
private:
 CThing* GetRegionTeleporter(const CRegion& region) const;           // 0x005FBEA0
 void TeleportToActiveRegion();                                       // 0x005FBFA0
public:
 void TeleportHeroToBoastingArea() const;                             // 0x005FB4B0
private:
 #endif
 #ifdef FABLE_RETAIL_CTCINVENTORYMAP_FRAMEUPDATE
 // Nested opt-in (chunk 1011, row 0x006061B0): the CTCBase slot-9 override (vtable 0x01257A9C slot 9 = 0x006061B0) and the two
 // region updaters it calls, Ego_r.pdb spellings: ?FrameUpdate@CTCInventoryMap@@UAEXXZ, ?UpdateVisitedRegions@CTCInventoryMap@@IAEXJJ@Z
 // (protected, 0x00605CE0) and ?UpdateRegionsCorrespondances@CTCInventoryMap@@AAEXJJ@Z (private, landed 0x005FC0C0). Declarations
 // only, no layout change (FrameUpdate overrides an existing slot).
public:
 virtual void FrameUpdate();                                          // 0x006061B0
protected:
 void UpdateVisitedRegions(long from_frame, long to_frame);           // 0x00605CE0
private:
 void UpdateRegionsCorrespondances(long from_frame, long to_frame);   // 0x005FC0C0
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS
 // Nested opt-in (chunk 1011, rows 0x005FCBE0 / 0x005FDDD0): Ego_r.pdb + FableWin.pdb
 // ?ConstructPastQuestsList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z (private, `ret 4`; Ego_r 0x0064F5D0) and
 // ?ConstructPastQuestsListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z (private, `ret 8`; Ego_r 0x00651F00).
 // Declarations only, no layout change.
private:
 NUISystem::CComponent* ConstructPastQuestsList(NUISystem::CComponent* parent);                                  // 0x005FCBE0
 NUISystem::CComponent* ConstructPastQuestsListPC(NUISystem::CComponent* container, NUISystem::CComponent* parent); // 0x005FDDD0
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW
 // Nested opt-in (chunk 1011b, rows 0x005FC2D0 / 0x005FC8A0): Ego_r.pdb + FableWin.pdb
 // ?ConstructQuestReviewList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z (private, `ret 4`; Ego_r 0x0064C6C0, FableWin
 // 0x02088CA0) and ?ConstructQuestReviewListPC@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@0@Z (private, `ret 8`; Ego_r
 // 0x0064CE90, FableWin 0x020AEC70): the active-quest twins of the past-quests pair, called from ConstructQuestsScreen(PC).
 // Declarations only, no layout change.
private:
 NUISystem::CComponent* ConstructQuestReviewList(NUISystem::CComponent* parent);                                  // 0x005FC2D0
 NUISystem::CComponent* ConstructQuestReviewListPC(NUISystem::CComponent* container, NUISystem::CComponent* parent); // 0x005FC8A0
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
 // Retail opt-in (chunk 851, rows 0x006001E0 / 0x005FE840): Ego_r.pdb + FableWin.pdb
 // ?ConstructRegionDescriptionShops@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@PAVCUIRegionInfo@1@_N3@Z (protected),
 // ?ConstructRegionExitMarkers@CTCInventoryMap@@AAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z (private) and
 // ?GetRelativePosOnMiniMap@CTCInventoryMap@@IBEXAAVCRegion@@ABVC3DVector@@AAVC2DVector@@@Z (protected, out of line at 0x005FBAA0) and
 // ?IsThingAsPosBeingDrawn@CTCInventoryMap@@IBE_NABVC3DVector@@@Z (protected, 0x005FABA0: the landed row's descriptive
 // IsPositionInsideClipEdges; the only PDB CTCInventoryMap member of that signature, and Ego_r's order IsTeleportValid <
 // GetMenuEntryFromCategory < IsThingAsPosBeingDrawn < GetMenuEntryFromCategoryPC is retail's 0x005FAA60 < 0x005FAA90 < 0x005FABA0 <
 // 0x005FAC20).
 // Declarations only, no layout change.
protected:
 void ConstructRegionDescriptionShops(NUISystem::CComponent*,CRegion&,CUIRegionInfo*,bool,bool);
 void GetRelativePosOnMiniMap(CRegion&,const C3DVector&,C2DVector&)const;
 bool IsThingAsPosBeingDrawn(const C3DVector&)const;
private:
 void ConstructRegionExitMarkers(NUISystem::CComponent*,CRegion&);
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION_ALL
 // Nested opt-in (chunk 2025, rows 0x00605570 / 0x00600B60): Ego_r.pdb + FableWin.pdb (both name sets)
 // ?ConstructRegionDescription@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@_N@Z (protected, `ret 0Ch`, 0x00605570),
 // ?ConstructRegionDescriptionQuests@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@_N@Z (protected, `ret 0Ch`,
 // 0x005FD5E0: UI_/PC_QUEST_NAME, *_CORE/OPTIONAL/VIGNETTE_ICON_AND_NAME templates),
 // ?ConstructRegionDescriptionBuyableBuildings@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@PAVCUIRegionInfo@1@_N3@Z
 // (protected, `ret 14h`, 0x00600B60: TEXT_GUI_MENU_HOUSE(S)_OWNED / SHOP(S)_OWNED / ..._FOR_SALE) and
 // ?ConstructRegionDescriptionMapKeys@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@_N@Z (protected, `ret 0Ch`,
 // 0x005FB7E0: UI_/PC_MAP_KEYS_CONTAINER). ConstructRegionDescription calls the other four in this order. Declarations only, no
 // layout change. Needs FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION.
protected:
 void ConstructRegionDescription(NUISystem::CComponent*,CRegion&,bool);
 void ConstructRegionDescriptionQuests(NUISystem::CComponent*,CRegion&,bool);
 void ConstructRegionDescriptionBuyableBuildings(NUISystem::CComponent*,CRegion&,CUIRegionInfo*,bool,bool);
 void ConstructRegionDescriptionMapKeys(NUISystem::CComponent*,CRegion&,bool);
private:
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_QUESTMARKERS
 // Retail opt-in (chunk 882, row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC): Ego_r.pdb + FableWin.pdb public members
 // ?CreateMiniMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z (retail 0x00603000),
 // ?CreateMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAV?$vector@VCRegion@@V?$allocator@VCRegion@@@std@@@std@@@Z
 // (0x005FF190) and ?GetRegionsToDisplay@CTCInventoryMap@@QAEXAAV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z (0x005FD410).
 // Declarations only, no layout change.
public:
 void CreateMiniMapMarkers(NUISystem::CComponent* parent, CRegion& region);
 void CreateMapMarkers(NUISystem::CComponent* parent, std::vector<CRegion>& regions);
 void GetRegionsToDisplay(std::set<long>& regions);
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_QUESTSSCREEN
 // Nested opt-in (chunk 2029, row 0x005FE090): Ego_r.pdb (donor 0x006522D0) + FableWin.pdb (0x02083ED0)
 // ?ConstructQuestsScreen@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z (public, `ret 4`; reloc_bindings.tsv binds it at
 // 0x005FE090 from NUISystem::CManager::Action 0x0041E6D3). It calls ConstructPastQuestsList / ConstructQuestReviewList (opt-ins
 // PASTQUESTS / QUESTREVIEW). Declaration only, no layout change.
public:
 NUISystem::CComponent* ConstructQuestsScreen(NUISystem::CComponent* parent);                                     // 0x005FE090
private:
#endif
public:
 static void CheckRetailLayout() {
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,VisitedRegions)==0x134);
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,LogBookNameTag)==0x150);
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,MiniMapMarkers)==0x18c);
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,SerialisedTeleporterUIDs)==0x200);
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,RegionsCorrespondances)==0x24c);
 }
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK
 // Retail opt-in (row 0x00607330): Ego_r.pdb / FableWin.pdb ?ConstructLogbookList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z
 // (public) and ?GetMenuEntryFromCategory@CTCInventoryMap@@IAEPAVCComponent@NUISystem@@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAV23@@Z
 // (protected, landed 0x005FAA90). Declarations only.
public:
 NUISystem::CComponent* ConstructLogbookList(NUISystem::CComponent* parent);                                          // 0x00607330
protected:
 NUISystem::CComponent* GetMenuEntryFromCategory(CHeroLogBook::CHeroLogBookEntry::ECategory category, NUISystem::CComponent* parent); // 0x005FAA90
public:
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x00619F40
public:
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_HISCREEN
// agentFAM13 (2026-11-12): the NHeroInformationScreens::CBase overrides this declaration lacks (the secondary vtable at +0x130,
// slots 0 GetScreenName / 1 GetPlayerMode / 2 GetScreenType / 3 IsInventoryScreen, engine/NHeroInformationScreens_CBase.h);
// spellings from the Ego_r / FableWin name sets. Declaration only (no layout change; the vtable exists).
public:
    virtual CWideString GetScreenName() const;
    virtual EPlayerMode GetPlayerMode() const;
    virtual NHeroInformationScreens::EScreenType GetScreenType() const;
    virtual bool IsInventoryScreen() const;
#endif // FABLE_RETAIL_CTCINVENTORYMAP_HISCREEN
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_MINIMAPMARKERS
 public:
 // Ego_r.pdb 0x00652B10 / FableWin.pdb 0x02093750 ?CreateMiniMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z (retail 0x00603000).
 void CreateMiniMapMarkers(NUISystem::CComponent* pminimap, CRegion& region);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap)==0x258);
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap::CUIMiniMapMarker)==28);
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap::CUIRegionInfo)==40);
#endif
