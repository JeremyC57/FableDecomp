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
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_PASTQUESTS
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_QUESTREVIEW
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION
class C3DVector;
namespace NUISystem { class CComponent; }
#endif
// Layout: original TPI2aa6d and native constructor allocation258. The selected
// serialiser independently addresses every used member at the offsets below.
class CTCInventoryMap : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
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
 unsigned char CategoriesList[12];
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
public:
 static void CheckRetailLayout() {
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,VisitedRegions)==0x134);
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,LogBookNameTag)==0x150);
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,MiniMapMarkers)==0x18c);
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,SerialisedTeleporterUIDs)==0x200);
  FABLE_STATIC_ASSERT(offsetof(CTCInventoryMap,RegionsCorrespondances)==0x24c);
 }
#ifdef FABLE_RETAIL_CTCINVENTORYMAP_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x00619F40
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap)==0x258);
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap::CUIMiniMapMarker)==28);
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap::CUIRegionInfo)==40);
#endif
