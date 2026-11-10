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
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
// chunk 886 (row 0x0041E6D3 NUISystem::CManager::Action): NInventory::CPerItemData (Ego_r.pdb, 40 bytes there; native offsets
// below with the 12-byte STLport vector), returned by ?GetPerItemDataForItem@CTCInventoryBase@NInventory@@QBEPBVCPerItemData@2@J@Z
// (0x005BE8D3; LOAD_WEAPON_MENU_ENTRY_NAME reads ->Augmentations at +0).
#include "stlport/fable_containers.h"
namespace NInventory {
class CPerItemData {
public:
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR
    // Nested opt-in (chunk 1002, CTCInventoryWeapons description rows 0x005C71AA / 0x005C6B26): Ego_r.pdb ??0CPerItemData@NInventory@@QAE@XZ,
    // expanded inline in retail: the vector and the three counts zeroed, QuestPersistent true, QuestPersistentRefCount -1
    // (`or [ebp-1Ch],-1 ; mov byte ptr [ebp-20h],1` for a local at [ebp-38h]); BestFishWeight is left unset.
    CPerItemData() : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(true), QuestPersistentRefCount(-1) {}
#endif
    std::vector<long> Augmentations;                    // +0x00
    long NumberOfKills;                                 // +0x0C
    long TimeWielded;                                   // +0x10
    long BestTrophyWitnessesAheadToDate;                // +0x14
    bool QuestPersistent;                               // +0x18
    long QuestPersistentRefCount;                       // +0x1C
    float BestFishWeight;                               // +0x20
};
}
#endif
// Layout: original donor328, native304 after the six vector reductions.
// Every original data member is retained; unused composite fields keep their
// original names and complete native storage. Native map secondary base is130.
class CGraphicDataBank;
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#include "fable_string.h"
class CThing;
namespace NInventory { class CItem; }
#endif
namespace NInventory {
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
class CCategory;
#endif
class CTCInventoryBase : public CTCBase {
public:
 long GetBoughtPrice(long) const; // original const member, native005bead5
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_ISOPEN
 // Nested opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): Ego_r.pdb / FableWin.pdb
 // ?IsInventoryOpen@CTCInventoryBase@NInventory@@QBE_NXZ, inline in retail (the caller tests InventoryOpen +0x58 in place:
 // `cmp byte ptr [ecx+58h],0`, 0x0041F5DF / 0x0041F641, before BuyItemPC / SellItemPC). No layout change.
 bool IsInventoryOpen() const { return InventoryOpen; }
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
 const CPerItemData* GetPerItemDataForItem(long itemDefIndex) const;   // 0x005BE8D3
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
 // Retail opt-in (chunk 867, row 0x005A8C66 CTCInventoryTrade::ConstructActualSpecialInventoryPC): Ego_r.pdb + FableWin.pdb
 // ?GetBuildingThing@CTCInventoryBase@NInventory@@QBEPAVCThing@@XZ, out of line in retail at 0x005BC4C4 (`mov eax,[ecx+0B8h]`:
 // pBuildingThing). The landed spellings GetShopThing / GetTradingThing / GetOwner for that address are propagated (NAME_FIXES).
 CThing* GetBuildingThing() const;                                   // 0x005BC4C4
 // Also chunk 867 (0x005A8C66): ?GetAllItems@CTCInventoryBase@NInventory@@QBEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@@Z
 // (0x005BF482): every owned (itemDefIndex, count) pair, the list the special-shop screens build their buy columns from.
 void GetAllItems(std::vector<std::pair<long, long> >& items) const;
 // Also chunk 867 (0x005AF64E BuyItem / 0x005B01FE SellItemPC): Ego_r.pdb + FableWin.pdb
 // ?IsSpaceInInventoryForItem@CTCInventoryBase@NInventory@@QBE_NJ@Z (0x005BF2ED; the catalog's GetItemDescription label there is a
 // propagated CItem name): whether the hero's inventory component can take one more of the item.
 bool IsSpaceInInventoryForItem(long itemDefIndex) const;              // 0x005BF2ED
#endif
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
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_SELECTION
 // Retail opt-in (chunk 1003, row 0x005B48A2 CTCInventoryClothing::OnItemSelected): Ego_r.pdb ?DeselectAll@CTCInventoryBase@NInventory@@IAEXXZ
 // (protected, out of line at 0x005BCCF1) and ?SelectItemWithDefIndex@CTCInventoryBase@NInventory@@IAEXJ@Z (protected, 0x005BDFB1). Declarations only.
protected:
 void DeselectAll();                                                                          // 0x005BCCF1
 void SelectItemWithDefIndex(long itemDefIndex);                                              // 0x005BDFB1
public:
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_ONINITIALACTIVATE
 // Retail opt-in (chunk 1002, row 0x005CB974 CTCInventoryWeapons::OnInitialActivate, which calls it qualified last): Ego_r.pdb
 // ?OnInitialActivate@CTCInventoryBase@NInventory@@UAEXXZ (the CTCBase slot-6 override, out of line at 0x005BD1EB).
public:
 virtual void OnInitialActivate();
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_CREATEOBJECT
 // Retail opt-in (chunk 1002, rows 0x005C4671 / 0x005CB2B0 / 0x005CB974 of CTCInventoryWeapons): Ego_r.pdb
 // ?CreateInventoryObject@CTCInventoryBase@NInventory@@IAEPAVCThing@@J@Z (protected, out of line at 0x005BE8F3).
protected:
 CThing* CreateInventoryObject(long itemDefIndex);
public:
#endif
 enum EInventorySelectionState { INVENTORY_SELECTION_STATE_CATEGORIES=0,INVENTORY_SELECTION_STATE_ITEMS=1,INVENTORY_SELECTION_STATE_MAX_STATES=2 };
 enum ESelectionState { SELECTION_STATE_NONE=0,SELECTION_STATE_OFF=1,SELECTION_STATE_ON=2,SELECTION_STATE_SEL=3,SELECTION_STATE_MAX=4 };
protected:
 virtual void OnSerialise(CPersistContext&,bool);
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
 // Retail opt-in (chunk 867, rows 0x005AF64E BuyItem / 0x005B01FE SellItemPC, which call slot 79 RemoveItemFromInventory on the
 // hero's category inventory through a CTCInventoryBase*): the class's own virtuals in retail vtable order (0x01253DBC, slots 28..87
 // after CTCBase's 0..27; the CTCInventoryTrade 0x01253034 / Weapons 0x01253FF4 / Clothing 0x0124356C / Quests 0x012585B4 tables
 // override the same slots). Names: Ego_r.pdb and FableWin.pdb ?..@CTCInventoryBase@NInventory@@U/M..; the slot of each is fixed
 // by its retail body or an override (76 GetCategoryNameFromAssociation: Quests 0x0061A3B0; 77 ConstructScreen: `xor eax,eax ; ret 4`,
 // Weapons / Clothing override; 70 ProcessQuestActivateAction: `xor al,al`, Quests 0x006223C0; 71 / 72 Increment / Decrement
 // QuestCardItemSelection: Quests 0x0061A1A0 / 0x0061A1D0 step g_testQuestIndex; 75 GetIsEnabled: `mov al,1`, Quests reads +0x14C;
 // 82 GetComponents: `lea eax,[ecx+0Ch]`; 34 OnItemSelectedInHand: `ret 8`; 46 SetInventorySelectionState: `ret 4`; 66 / 67 the
 // White button pair before ProcessTradeAction, Quests overrides 67 = Released). The remaining empty `ret` stubs take the remaining
 // Ego_r / FableWin names by elimination: 28 UpdateMenuScrollButtonsCategory, 29 UpdateMenuScrollButtonsItem (the Category-before-
 // Item order of slots 42..45), 37 OnChangeCategory (every derived class overrides it with its own `ret`), 69 ProcessAugmentAction --
 // these four placements are unproven. Requires FABLE_RETAIL_CTCINVENTORYBASE (the typed Components vector).
public:
 virtual void UpdateMenuScrollButtonsCategory();                                            // 28 0x005BC589 (ret; placement unproven)
 virtual void UpdateMenuScrollButtonsItem();                                                // 29 0x005BC58A (ret; placement unproven)
 virtual void OpenInventory();                                                              // 30 0x005BD4EA
 virtual void CloseInventory();                                                             // 31 0x005BD597
 virtual void AddCloseInventoryEvent();                                                     // 32 0x005BC527
 virtual void OnItemSelected(long itemDefIndex, bool& handled) = 0;                         // 33 _purecall (Trade 0x005B3065)
 virtual void OnItemSelectedInHand(CThing& item, bool& handled);                            // 34 0x004E7D3E (ret 8)
 virtual void OnItemDeselected(long itemDefIndex, bool& handled);                           // 35 0x0057A230
 virtual void OnItemHighlighted(long itemDefIndex);                                         // 36 0x005BC4C0
 virtual void OnChangeCategory();                                                           // 37 0x005BC4C3 (ret; placement unproven)
 virtual void ProcessInventoryInputUp();                                                    // 38 0x005BDD8C
 virtual void ProcessInventoryInputDown();                                                  // 39 0x005BDDB8
 virtual void ProcessInventoryInputLeft();                                                  // 40 0x005BC638
 virtual void ProcessInventoryInputRight();                                                 // 41 0x005BC649
 virtual float GetInitialCategoryRepeatDelay() const;                                       // 42 0x005BD404
 virtual float GetContinuedCategoryRepeatDelay() const;                                     // 43 0x005BD427
 virtual float GetInitialItemRepeatDelay() const;                                           // 44 0x005BD44A
 virtual float GetContinuedItemRepeatDelay() const;                                         // 45 0x005BD46D
 virtual void SetInventorySelectionState(EInventorySelectionState state);                   // 46 0x005BC586 (ret 4)
 virtual CThing* GetItemOfTypeInInventory(long itemDefIndex);                               // 47 0x005BF3F1
 virtual bool GetInventoryShoppingValidity();                                               // 48 0x005BE119
 virtual bool GetInventoryGuildValidity();                                                  // 49 0x005BDC77
 virtual void ProcessButtonAPressed();                                                      // 50 0x005BC65A
 virtual void ProcessButtonBPressed();                                                      // 51 0x005BC66D
 virtual void ProcessButtonXPressed();                                                      // 52 0x005BC68D
 virtual void ProcessButtonYPressed();                                                      // 53 0x005BC690
 virtual void ProcessButtonAHeld();                                                         // 54 0x005BC65B
 virtual void ProcessButtonBHeld();                                                         // 55 0x005BC66E
 virtual void ProcessButtonXHeld();                                                         // 56 0x005BC68E
 virtual void ProcessButtonYHeld();                                                         // 57 0x005BC691
 virtual void ProcessButtonAReleased();                                                     // 58 0x005BC65C
 virtual void ProcessButtonBReleased();                                                     // 59 0x005BC66F
 virtual void ProcessButtonXReleased();                                                     // 60 0x005BC68F
 virtual void ProcessButtonYReleased();                                                     // 61 0x005BC692
 virtual void ProcessDPadUpReleased();                                                      // 62 0x005BC693
 virtual void ProcessDPadDownReleased();                                                    // 63 0x005BC69B
 virtual void ProcessDPadLeftReleased();                                                    // 64 0x005BC6A3
 virtual void ProcessDPadRightReleased();                                                   // 65 0x005BC6AB
 virtual void ProcessButtonWhitePressed();                                                  // 66 0x004E7D41
 virtual void ProcessButtonWhiteReleased();                                                 // 67 0x004E7D42 (Quests 0x0061A160)
 virtual void ProcessTradeAction();                                                         // 68 0x004E7D43 (Trade 0x005AEBFE)
 virtual void ProcessAugmentAction();                                                       // 69 0x004E7D44 (ret; placement unproven)
 virtual bool ProcessQuestActivateAction();                                                 // 70 0x004E7D45 (xor al,al)
 virtual void IncrementQuestCardItemSelection();                                            // 71 0x004E7D48
 virtual void DecrementQuestCardItemSelection();                                            // 72 0x004E7D49
 virtual void ReturnAllConfiscatedItems();                                                  // 73 0x005BF91A
 virtual void RemoveAllConfiscatedItems();                                                  // 74 0x005BF969
 virtual bool GetIsEnabled() const;                                                         // 75 0x004E7D4A (mov al,1)
 virtual CWideString GetCategoryNameFromAssociation(CThing* thing) const;                   // 76 0x004E7D4D (the empty string)
 virtual NUISystem::CComponent* ConstructScreen(NUISystem::CComponent* parent);             // 77 0x0057A233 (xor eax,eax ; ret 4)
 virtual void GetItemDescription(CWideString& text, long itemDefIndex, bool a) const;       // 78 0x005BC6B3
 virtual void RemoveItemFromInventory(long itemDefIndex, long count);                       // 79 0x005BF3C6 (Trade 0x0059C747)
 virtual bool AddItemToInventory(long itemDefIndex, long count, bool a, bool b, long c, bool d);   // 80 0x005BF654 (Trade 0x0059C6DD)
protected:
 virtual bool AddItemToInventory(CThing& item, bool a, bool b, long c, bool d);             // 81 0x005BF32D (Trade 0x0059C710)
public:
 virtual std::vector<NUISystem::CComponent*>& GetComponents();                              // 82 0x004E7D62 (lea eax,[ecx+0Ch])
protected:
 virtual void OnItemReturnedToInventory(CThing& item);                                      // 83 0x005BF4ED
 virtual void OnItemAdded(long itemDefIndex, bool a, bool b);                               // 84 0x005BE028
 virtual void ConfiscateItems(const std::vector<CItem>& items);                             // 85 0x005BFB01
public:
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYBASE
 // Retail opt-in (chunk 735, row 0x00577AC0 CTCInventoryExperience::OpenInventoryInternalPC): Ego_r.pdb
 // ?OpenInventory@CTCInventoryBase@NInventory@@UAEXXZ (public virtual, out of line at 0x005BD4EA; the row calls it qualified)
 // and the Components member typed as its PDB vector<NUISystem::CComponent*> (STLport 12 bytes; push_back(const T&) out of
 // line at 0x00486024, as the landed CTCInventoryTrade::OpenInventoryPC 0x005AE86B also calls it).
public:
#ifndef FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
 virtual void OpenInventory();
#endif
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
#ifndef FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
 virtual void RemoveAllConfiscatedItems();                 // slot 74, 0x005BF969 (Ego_r.pdb UAEXXZ)
#endif
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
