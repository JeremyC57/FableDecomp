#ifndef FABLE_ENGINE_CTCINVENTORYMAP_H
#define FABLE_ENGINE_CTCINVENTORYMAP_H
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CHeroLogBook.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
class CRegion;
#ifdef FABLE_RETAIL_CTCINVENTORYMAP
class C3DVector;
#endif
#include "engine/NHeroInformationScreens_CBase.h"
// Layout: original TPI2aa6d and native constructor allocation258. The selected
// serialiser independently addresses every used member at the offsets below.
class CTCInventoryMap : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
 void AddLogBookSaveInfo(const CWideString&,const CWideString&,const CWideString&,CHeroLogBook::CHeroLogBookEntry::ECategory);
 class CUIMiniMapMarker {
 public: long Region;CCharString UIDefName;C2DVector Position;long DefNameLength,GraphicIndex;float Angle;
  CUIMiniMapMarker():Region(0),UIDefName(""),Position(0.0f,0.0f),DefNameLength(0),GraphicIndex(0),Angle(0.0f){}
 };
 class CUIRegionInfo {
 public:long Region,Shops,BarberShops,TitleShops,TatooShops,ShopsForSale,HousesForSale,ShopsOwned,HousesOwned,Taverns;
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
#ifdef FABLE_RETAIL_CTCINVENTORYMAP
 // Retail branch (chunk 729): Ego_r.pdb / FableWin.pdb members called or defined by CreateMapMarkers 0x005FF190 and
 // CreateMiniMapMarkers 0x00603000. Declarations only: no layout change.
 CThing* GetRegionTeleporter(const CRegion&) const;          // 0x005FBEA0 ?GetRegionTeleporter@CTCInventoryMap@@ABEPAVCThing@@ABVCRegion@@@Z
 void ConstructRegionExitMarkers(NUISystem::CComponent*, CRegion&);    // 0x005FE840 ?ConstructRegionExitMarkers@CTCInventoryMap@@AAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z
protected:
 // Ego_r.pdb protected (I) members: 0x005FBAA0 (catalog "GetRelativePosOnMiniMap", same spelling) and 0x005FABA0 (catalog
 // "IsPositionInsideClipEdges", in neither PDB; the body is the four MiniMapIs* tests against Z 51 / Y 4740 on a const C3DVector&,
 // and IsThingAsPosBeingDrawn is the only bool(const C3DVector&) const member of the class in both name sets).
 void GetRelativePosOnMiniMap(CRegion&, const C3DVector&, C2DVector&) const;   // 0x005FBAA0 ?GetRelativePosOnMiniMap@CTCInventoryMap@@IBEXAAVCRegion@@ABVC3DVector@@AAVC2DVector@@@Z
 bool IsThingAsPosBeingDrawn(const C3DVector&) const;                  // 0x005FABA0 ?IsThingAsPosBeingDrawn@CTCInventoryMap@@IBE_NABVC3DVector@@@Z
public:
 void CreateMapMarkers(NUISystem::CComponent*, std::vector<CRegion>&);   // 0x005FF190 ?CreateMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAV?$vector@VCRegion@@...@Z
 void CreateMiniMapMarkers(NUISystem::CComponent*, CRegion&);            // 0x00603000 ?CreateMiniMapMarkers@CTCInventoryMap@@QAEXPAVCComponent@NUISystem@@AAVCRegion@@@Z
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
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap)==0x258);
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap::CUIMiniMapMarker)==28);
FABLE_STATIC_ASSERT(sizeof(CTCInventoryMap::CUIRegionInfo)==40);
#endif
