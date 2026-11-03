#ifndef FABLE_ENGINE_NINVENTORY_CTCINVENTORYBASE_H
#define FABLE_ENGINE_NINVENTORY_CTCINVENTORYBASE_H
#include "engine/CTCBase.h"
// Layout: original donor328, native304 after the six vector reductions.
// Every original data member is retained; unused composite fields keep their
// original names and complete native storage. Native map secondary base is130.
class CGraphicDataBank;
#ifdef FABLE_RETAIL_CTCINVENTORYBASE
class CRegion;
#endif
namespace NInventory {
class CTCInventoryBase : public CTCBase {
public:
 long GetBoughtPrice(long) const; // original const member, native005bead5
#ifdef FABLE_RETAIL_CTCINVENTORYBASE
 // chunk 729 (row 0x00603000): Ego_r.pdb / FableWin.pdb ?GetMiniMapGraphic@CTCInventoryBase@NInventory@@QBEKAAVCRegion@@@Z. Declaration only.
 unsigned long GetMiniMapGraphic(CRegion&) const;                // 0x005BEB57
#endif
 enum EInventorySelectionState { INVENTORY_SELECTION_STATE_CATEGORIES=0,INVENTORY_SELECTION_STATE_ITEMS=1,INVENTORY_SELECTION_STATE_MAX_STATES=2 };
 enum ESelectionState { SELECTION_STATE_NONE=0,SELECTION_STATE_OFF=1,SELECTION_STATE_ON=2,SELECTION_STATE_SEL=3,SELECTION_STATE_MAX=4 };
protected:
 virtual void OnSerialise(CPersistContext&,bool);
 unsigned char Components[12];
 bool FirstRenderFrame;
 EInventorySelectionState InventorySelectionState;
 unsigned char Categories[12];
 long HighlightedItemNumOwned,HighlightedItemMaxOwned;
 bool CanItemBeQuickAssigned,CanItemBeUsed;
 ESelectionState MenuArrowHighlightsCategoryUp,MenuArrowHighlightsCategoryDn;
 ESelectionState MenuArrowHighlightsItemUp,MenuArrowHighlightsItemDn;
 ESelectionState TriggerStateLeft,TriggerStateRight;
 double LastRenderTime;
 bool InventoryOpen;
 unsigned char _pad_after_InventoryOpen[3];
 unsigned char OldIntervalDesc[8];
 bool OldIntervalDescInitialised;
 unsigned char _pad_after_OldIntervalDescInitialised[3];
private:
 unsigned char PDef[4];
 long GlobalDefIndex,SelectedCategoryIndex;
 const CGraphicDataBank* GraphicManager;
 unsigned char SerialisedItems[8],SerialisedSelectedItems[8];
 bool Dying,CategoryMoving;
 float CategoryMovement,CategoryMovementDirection;
 unsigned char MenuDisplayListItem[12],MenuDisplayListCategory[12],ConfiscatedItems[12];
 CThing* pBuildingThing;
 unsigned char InventoryAnimRenderBox[16];
 unsigned char InventoryAnimCurrentRHSet[24],InventoryAnimLastRHSet[24];
 float InventoryAnimCurrentXRotationAngle,InventoryAnimLastXRotationAngle;
 float InventoryAnimCurrentYRotationAngle,InventoryAnimLastYRotationAngle;
 float InventoryAnimCurrentZRotationAngle,InventoryAnimLastZRotationAngle;
 float InventoryAnimXRotationAngleGain,InventoryAnimYRotationAngleGain,InventoryAnimZRotationAngleGain;
 bool EnableAnimXAxis,EnableAnimZAxis;
 long InventoryAnimThingDefIndex,RHSetInvItemDefIndex,ParentMapNumber;
};
}
FABLE_STATIC_ASSERT(sizeof(NInventory::CTCInventoryBase)==0x130);
#endif
