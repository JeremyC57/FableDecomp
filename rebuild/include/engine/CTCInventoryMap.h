#ifndef FABLE_ENGINE_CTCINVENTORYMAP_H
#define FABLE_ENGINE_CTCINVENTORYMAP_H
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CHeroLogBook.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
class CRegion;
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
