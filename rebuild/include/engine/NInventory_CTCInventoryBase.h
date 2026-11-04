#ifndef FABLE_ENGINE_NINVENTORY_CTCINVENTORYBASE_H
#define FABLE_ENGINE_NINVENTORY_CTCINVENTORYBASE_H
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCINVENTORYBASE
#include "stlport/fable_containers.h"
namespace NUISystem { class CComponent; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX
#include "engine/C2DBoxF.h"
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
// chunk 770 (row 0x005BF969 RemoveAllConfiscatedItems): ConfiscatedItems typed as its Ego_r.pdb vector<NInventory::CItem>
// (STLport 12 bytes at retail +0xAC; erase(first, last) out of line at 0x005C1ECC) and NInventory::CItem (Ego_r.pdb 0x14 bytes:
// ItemType +0, NumberOfItems +4, PPerItemData +8, AverageBoughtPrice +0x10; out-of-line ??0 (copy) 0x005C08E6, ??4, ??1).
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NInventory {
class CPerItemData;
class CItem {
public:
    CItem(const CItem&);
    ~CItem();
    CItem& operator=(const CItem&);
    long ItemType;                                      // +0x00
    long NumberOfItems;                                 // +0x04
    CCountedPointer<CPerItemData> PPerItemData;         // +0x08
    long AverageBoughtPrice;                            // +0x10
};
}
#endif
// Layout: original donor328, native304 after the six vector reductions.
// Every original data member is retained; unused composite fields keep their
// original names and complete native storage. Native map secondary base is130.
class CGraphicDataBank;
namespace NInventory {
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
class CCategory;
#endif
class CTCInventoryBase : public CTCBase {
public:
 long GetBoughtPrice(long) const; // original const member, native005bead5
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
 // chunk 764 (row 0x005BE284): Ego_r.pdb ?GetSelectedCategory@CTCInventoryBase@NInventory@@QAEPAVCCategory@2@XZ (0x005BCACE),
 // ?GetNumberOfItemsOfTypeInInventory@CTCInventoryBase@NInventory@@QBEJJ@Z (const, 0x005BDF08) and the protected
 // ?SetNumberOfSelectedItemOwned@CTCInventoryBase@NInventory@@IAEXJ@Z (0x005BE284).
 CCategory* GetSelectedCategory();
 long GetNumberOfItemsOfTypeInInventory(long itemDefIndex) const;
protected:
 void SetNumberOfSelectedItemOwned(long itemDefIndex);
public:
#endif
 enum EInventorySelectionState { INVENTORY_SELECTION_STATE_CATEGORIES=0,INVENTORY_SELECTION_STATE_ITEMS=1,INVENTORY_SELECTION_STATE_MAX_STATES=2 };
 enum ESelectionState { SELECTION_STATE_NONE=0,SELECTION_STATE_OFF=1,SELECTION_STATE_ON=2,SELECTION_STATE_SEL=3,SELECTION_STATE_MAX=4 };
protected:
 virtual void OnSerialise(CPersistContext&,bool);
#ifdef FABLE_RETAIL_CTCINVENTORYBASE
 // Retail opt-in (chunk 735, row 0x00577AC0 CTCInventoryExperience::OpenInventoryInternalPC): Ego_r.pdb
 // ?OpenInventory@CTCInventoryBase@NInventory@@UAEXXZ (public virtual, out of line at 0x005BD4EA; the row calls it qualified)
 // and the Components member typed as its PDB vector<NUISystem::CComponent*> (STLport 12 bytes; push_back(const T&) out of
 // line at 0x00486024, as the landed CTCInventoryTrade::OpenInventoryPC 0x005AE86B also calls it).
public:
 virtual void OpenInventory();
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX
 // chunk 751 (row 0x005BC58F): Ego_r.pdb ?SetInventoryAnimRenderBox@CTCInventoryBase@NInventory@@QAEXABVC2DBoxF@@@Z and the
 // member typed as its PDB C2DBoxF (same 16 bytes at +0xBC).
 void SetInventoryAnimRenderBox(const C2DBoxF& box);
#endif
protected:
 std::vector<NUISystem::CComponent*> Components;
#else
 unsigned char Components[12];
#endif
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
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
 unsigned char MenuDisplayListItem[12],MenuDisplayListCategory[12];
 std::vector<CItem> ConfiscatedItems;
public:
 virtual void RemoveAllConfiscatedItems();                 // slot 74, 0x005BF969 (Ego_r.pdb UAEXXZ)
private:
#else
 unsigned char MenuDisplayListItem[12],MenuDisplayListCategory[12],ConfiscatedItems[12];
#endif
 CThing* pBuildingThing;
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX
 C2DBoxF InventoryAnimRenderBox;
#else
 unsigned char InventoryAnimRenderBox[16];
#endif
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
