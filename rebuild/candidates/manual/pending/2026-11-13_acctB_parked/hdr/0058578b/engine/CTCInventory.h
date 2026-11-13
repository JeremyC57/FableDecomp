#ifdef FABLE_RETAIL_EXPRESSION_FRAME
#ifndef FABLE_ENGINE_CTCINVENTORY_RETAIL_H
#define FABLE_ENGINE_CTCINVENTORY_RETAIL_H
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CRightHandedSet.h"
#include "stlport/fable_containers.h"
class CInventoryItemDef;class CFadeInFadeOutBase;class CEnginePrimitiveHandle;class CEnginePrimitivePrepareInfo;class CPersistContext;class CCharString;class CWideString;
namespace NUISystem{class CComponent;class CList;class CEditBox;}
#ifdef _MSC_VER
enum EContextSensitiveOpinionExpression;
#else
enum EContextSensitiveOpinionExpression{};
#endif
// Layout: complete original Ego20fbd/FW22e6d; native552 after genuine container reductions.
class CTCInventory:public NInventory::CTCInventoryBase,public NHeroInformationScreens::CBase{
public:class CContextSensitiveItem;
#ifdef _MSC_VER
enum CURRENT_OPEN_MENU;
#else
enum CURRENT_OPEN_MENU{};
#endif
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_INVENTORY;}
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 static const CCharString & FABLE_FASTCALL GetItemDescriptionString();
 static const CCharString & FABLE_FASTCALL GetItemNameTableString();
 static const CCharString & FABLE_FASTCALL GetItemIconString();
 CTCInventory(const CTCInventory & p0);
 CTCInventory(CThing & p0);
 virtual ~CTCInventory();
 virtual void OnKill();
 virtual void Copy(const CTCBase & p0);
 virtual void OnCreate();
 virtual void InitialActivate();
 virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0);
 virtual void OnItemSelected(long p0, bool & p1);
 virtual void OnItemSelectedInHand(CThing & p0, bool & p1);
 virtual void OnItemHighlighted(long p0);
 virtual void OpenInventory();
 virtual void CloseInventory();
 virtual void GlobalUpdate(bool p0);
 virtual bool IsComponentSerialisable() const;
 virtual void OnSerialise(CPersistContext & p0, bool p1);
 virtual void OnChangeCategory();
 virtual void GetItemDescription(CWideString & p0, long p1, bool p2) const;
 virtual const std::vector<long,std::allocator<long> > & GetInitialItems() const;
 void OpenQuickAccessMenu();
 void ForceContextSensitiveUpdate();
 bool IsButtonHeldForExpression(const CCharString & p0) const;
 CDefPointer<CInventoryItemDef const > GetQuickAccessItemDef(long p0) const;
 long GetQuickAccessItem(long p0) const;
 void SetHighlightedItemAsQuickAccessItem(long p0);
 void SetThingAsQuickAccessItem(long p0, long p1, bool p2);
 bool IsQuickAccessItemUsable(long p0) const;
 void UseQuickAccessItem(long p0);
 long GetQuickAccessItemGraphicIndex(long p0) const;
 long GetQuickAccessItemButtonIndex(long p0) const;
 long GetQuickAccessIndexFromButtonIndex(long p0) const;
 long GetQuickAccessNextFreeSlot(long p0) const;
 long GetQuickAccessItemCount() const;
 void RemoveQuickAccessItem(long p0);
 void UseQuickAccessExpressionInCutscene(long p0);
 void UpdateContextSensitiveItems();
 void UpdateContextSensitiveItemFades(const CEnginePrimitivePrepareInfo & p0);
 void UseContextSensitiveItem(long p0);
 long GetContextSensitiveItemGraphicIndex(long p0) const;
 float GetContextSensitiveFadeAlpha(long p0) const;
 long GetContextSensitiveItemButtonIndex(long p0) const;
 bool AddContextSensitivePreferredItem(const CCharString & p0, long p1, long p2);
 bool RemoveContextSensitivePreferredItem(const CCharString & p0);
 static long FABLE_FASTCALL GetContextSensitivePreferredItemPriority(long p0);
 static long FABLE_FASTCALL GetContextSensitivePreferredItemButtonIndex(long p0);
 bool IsContextSensitiveOpinionExpressionAvailable() const;
 virtual CWideString GetScreenName() const;
 virtual EPlayerMode GetPlayerMode() const;
 virtual NHeroInformationScreens::EScreenType GetScreenType() const;
 virtual void ProcessButtonAReleased();
 virtual void ProcessButtonBReleased();
 virtual void ProcessButtonXReleased();
 virtual void ProcessButtonYReleased();
 virtual void ProcessDPadUpReleased();
 virtual void ProcessDPadDownReleased();
 virtual void ProcessDPadLeftReleased();
 virtual void ProcessDPadRightReleased();
 virtual void ProcessInventoryInputLeft();
 virtual void ProcessInventoryInputRight();
 bool GetQuickAccessAssign() const;
 void SetQuickAccessAssign(bool p0);
 bool GetQuickAccessRemove() const;
 void SetQuickAccessRemove(bool p0);
 NUISystem::CComponent * GetItemsListUIComponent(NUISystem::CComponent * p0);
 void GetStyleCardsListUIComponent(long p0, long p1, long p2, NUISystem::CComponent * p3);
 void ConstructStyleEntry(NUISystem::CComponent * p0, std::vector<std::pair<long,long>,std::allocator<std::pair<long,long> > > & p1, const CCharString & p2, NUISystem::CComponent * p3);
 NUISystem::CComponent * GetItemsListUIComponentPC(NUISystem::CComponent * p0);
 void GetStyleCardsListUIComponentPC(long p0, long p1, long p2, NUISystem::CComponent * p3);
 void ConstructStyleEntryPC(NUISystem::CComponent * p0, std::vector<std::pair<long,long>,std::allocator<std::pair<long,long> > > & p1, NUISystem::CComponent * p2, const CCharString & p3, NUISystem::CComponent * p4);
 long GetThingToAssign();
 void SetThingToAssign(long p0);
 void SetThingSlotToSwap(long p0);
 long GetThingSlotToSwap();
 void SetComponentSlotToSwap(NUISystem::CComponent * p0);
 NUISystem::CComponent * GetComponentSlotToSwap();
 void AssignThingPC(long p0, unsigned long p1, NUISystem::CComponent * p2);
 void AssignThingSwappingPC(long p0, unsigned long p1, long p2, NUISystem::CComponent * p3, NUISystem::CComponent * p4);
 void UnAssignThingPC(unsigned long p0, NUISystem::CComponent * p1);
 void RevertVideoChanges();
 virtual void AddCloseInventoryEvent();
 void ResetPrimitiveHandles();
 virtual NUISystem::CComponent * ConstructScreen(NUISystem::CComponent * p0);
 void ConstructStyleCardsScreen(long p0, long p1, long p2, NUISystem::CComponent * p3);
 virtual NUISystem::CComponent * ConstructScreenPC(NUISystem::CComponent * p0);
 void ConstructStyleCardsScreenPC(long p0, long p1, long p2, NUISystem::CComponent * p3);
 void ConstructItemDescription(CWideString & p0, long p1);
 void ConstructItemCategoryList(CWideString & p0, NUISystem::CList * p1, std::vector<std::pair<long,long>,std::allocator<std::pair<long,long> > > & p2, NUISystem::CComponent * p3, const CCharString & p4, const CCharString & p5, bool p6);
 void ContructItemLists(std::map<long,long,std::less<long>,std::allocator<std::pair<long const ,long> > > & p0, NUISystem::CList * p1, NUISystem::CComponent * p2, CCountedPointer<NUISystem::CComponent> & p3);
 void ConstructItemEntry(long p0, long p1, NUISystem::CList * p2, NUISystem::CComponent * p3, CCountedPointer<NUISystem::CComponent> & p4);
 void ConstructItemCategoryListPC(CWideString & p0, NUISystem::CList * p1, std::vector<std::pair<long,long>,std::allocator<std::pair<long,long> > > & p2, NUISystem::CComponent * p3, const CCharString & p4, const CCharString & p5, bool p6);
 void ContructItemListsPC(std::map<long,long,std::less<long>,std::allocator<std::pair<long const ,long> > > & p0, NUISystem::CList * p1, NUISystem::CComponent * p2, CCountedPointer<NUISystem::CComponent> & p3);
 void ConstructItemEntryPC(long p0, long p1, NUISystem::CList * p2, NUISystem::CComponent * p3, CCountedPointer<NUISystem::CComponent> & p4);
 std::map<long,NUISystem::CComponent *,std::less<long>,std::allocator<std::pair<long const ,NUISystem::CComponent *> > > & GetItemsMenuEntries();
 std::map<long,NUISystem::CComponent *,std::less<long>,std::allocator<std::pair<long const ,NUISystem::CComponent *> > > & GetAssignedIcons();
 const std::vector<long,std::allocator<long> > & GetQuickAccessItems() const;
 std::vector<long,std::allocator<long> > & GetQuickAccessItems();
 long GetCSOpinionExpressionCount(EContextSensitiveOpinionExpression p0) const;
 long GetLastOpinionExpression(EContextSensitiveOpinionExpression p0) const;
 long GetLastButOneOpinionExpression(EContextSensitiveOpinionExpression p0) const;
 virtual void RemoveItemFromInventory(long p0, long p1);
 void OpenPCInventory();
 void ClosePCInventory(bool p0);
 void OpenPCMapMenu();
 void ClosePCMapMenu();
 void OpenPCSkillsMenu();
 void ClosePCSkillsMenu();
 void OpenPCInventoryMenu();
 void ClosePCInventoryMenu();
 void OpenPCOptionsMenu();
 void ClosePCOptionsMenu();
 void OpenPCPhotojournal();
 bool HasScreenshots();
 void ClosePCPhotojournal();
 void OpenPCScoreboard();
 void ClosePCScoreboard();
 void OpenPCStatusMenu();
 void ClosePCStatusMenu();
 void DeleteMenuComponents();
 void StartGameRendering();
 void RenderGrayedScreen(float p0);
 void CreateEditBoxActions(NUISystem::CComponent * p0, const CCharString & p1, const CCharString & p2, const CWideString & p3, bool p4, const CWideString & p5);
 void DeactivateAllEditBoxes(bool p0);
 void SetEditBoxValues();
 NUISystem::CComponent * GetMousePointer();
 bool IsQuitMenuReceivingInput() const;
 bool IsSaveMenuReceivingInput() const;
 bool IsLoadMenuReceivingInput() const;
 CTCInventory::CURRENT_OPEN_MENU GetCurrentOpenMenu();
 void OpenInventoryShortcut(long p0);
 void SetScreenToShortcut(long p0);
 long GetScreenToShortcut() const;
 void AdjustVideoOptionsSliders();
 void AdjustAudioOptionsSliders();
 void AdjustGameplayOptionsSliders();
protected:
 virtual void OnItemAdded(long p0, bool p1, bool p2);
private:
 CCountedPointer<NUISystem::CComponent> CreateMenuDesc(const CCharString & p0);
 CCountedPointer<NUISystem::CComponent> CreateMenuDescPC(const CCharString & p0);
 long ThingToAssign;
 long ThingSlotToSwap;
 NUISystem::CComponent * PComponentSlotToSwap;
 long ScreenToShortcut;
 std::list<NUISystem::CEditBox *,std::allocator<NUISystem::CEditBox *> > EditBoxes;
 std::vector<long,std::allocator<long> > QuickAccessItems;
 std::vector<long,std::allocator<long> > ContextSensitiveItems;
 std::vector<CFadeInFadeOutBase,std::allocator<CFadeInFadeOutBase> > ContextSensitiveFadeTimes;
 static std::vector<CTCInventory::CContextSensitiveItem,std::allocator<CTCInventory::CContextSensitiveItem> > ContextSensitivePreferredItems;
 std::vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle> > InventoryMainPrimitiveHandles;
 std::vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle> > QuickAccessPrimitiveHandles;
 std::vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle> > QuickAccessUnusableIconPrimitiveHandles;
 CRightHandedSet OldObjectRHSet;
 float OldRotationAngle;
 CRightHandedSet CurrentObjectRHSet;
 float CurrentRotationAngle;
 long ItemRHSetsApplyTo;
 bool EnableQuickAccessAssign;
 bool EnableQuickAccessRemove;
 bool QuickAccessFlashState;
 long QuickAccessFlashTimer;
 bool ContextSensitiveUpdate;
 std::vector<long,std::allocator<long> > NumberOfCSOpinionExpressions;
 std::vector<long,std::allocator<long> > LastCSOpinionExpression;
 std::vector<long,std::allocator<long> > LastButOneCSOpinionExpression;
 std::map<long,NUISystem::CComponent *,std::less<long>,std::allocator<std::pair<long const ,NUISystem::CComponent *> > > ItemsMenuEntries;
 std::map<long,NUISystem::CComponent *,std::less<long>,std::allocator<std::pair<long const ,NUISystem::CComponent *> > > AssignedIcons;
 NUISystem::CComponent * PMouse_Pointer;
 NUISystem::CComponent * PTop_Level_List;
 CTCInventory::CURRENT_OPEN_MENU CurrentOpenMenu;
 float AccumFadeTime;
public:
 CTCInventory & operator=(const CTCInventory & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCInventory)==552);
#endif
#else
#ifdef FABLE_RETAIL_CTCINVENTORY
#ifndef FABLE_ENGINE_CTCINVENTORY_RETAIL_H
#define FABLE_ENGINE_CTCINVENTORY_RETAIL_H
// Retail opt-in (chunk 849, row 0x0058E99F OpenInventoryShortcut; the CTCInventoryExperience pattern): the thing component on its
// two RTTI bases (.?AVCTCInventory@@: NInventory::CTCInventoryBase at +0, sizeof 0x130, and NHeroInformationScreens::CBase at
// +0x130, rtti_bases.tsv), then the members at the generated retail offsets below (Ego_r.pdb names; STLport 4.6 sizes). Retail
// reads ParentThing +0x04 (CTCBase), Components +0x0C..+0x10 (CTCInventoryBase, FABLE_RETAIL_CTCINVENTORYBASE) and
// PTop_Level_List +0x218 (`cmp dword ptr [esi+218h],0` at 0x0058E9EC). Methods: Ego_r.pdb / FableWin.pdb spellings (all public
// QAEXXZ / QAEXJ@Z): OpenInventoryShortcut, DeleteMenuComponents 0x00587A41, OpenPCInventoryMenu 0x0058807D, OpenPCMapMenu
// 0x00589A30, OpenPCSkillsMenu 0x00587BFF, OpenPCStatusMenu 0x0058A763 (the jump table at 0x0058EF31: cases 0-2 / 4 / 5,8 / 3,6,7).
// Define FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CTCINVENTORYBASE with it.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#ifdef FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#endif
namespace NUISystem { class CComponent; class CList; }
class CWideString; class CCharString;
#ifdef FABLE_RETAIL_CTCINVENTORY_ITEMENTRY
template <class T> class CCountedPointer;
#endif
class CTCInventory : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
#ifdef FABLE_RETAIL_CTCINVENTORY_OPENINVENTORY
    // Retail opt-in (row 0x0058578B): Ego_r.pdb / FableWin.pdb ?OpenInventory@CTCInventory@@UAEXXZ, the console inventory's top-level
    // menu (CTCInventory vtable 0x01250DE4 slot 30, the base NInventory::CTCInventoryBase::OpenInventory slot). Declaration only.
    virtual void OpenInventory();                                   // 0x0058578B
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_REVERTVIDEOCHANGES
    // agentQ7 (2026-11-11, row 0x0047E9AE CTCInGameMenu::GlobalUpdate): Ego_r.pdb / FableWin.pdb ?RevertVideoChanges@CTCInventory@@QAEXXZ,
    // out of line in retail at 0x00582754 (the in-game menu's 15-second video-options timeout calls it). Declaration only.
    void RevertVideoChanges();                                                                    // 0x00582754
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_PCMENUS
    // Nested opt-in (chunk 867, row 0x00590F0B GetItemsListUIComponentPC; needs FABLE_RETAIL_CTCINVENTORY_QUICKACCESS for the
    // vector include): the PC inventory screen's items page and its per-category list builder, FableWin.pdb / Ego_r.pdb spellings
    //   ?GetItemsListUIComponentPC@CTCInventory@@QAEPAVCComponent@NUISystem@@PAV23@@Z                              0x00590F0B
    //   ?ConstructItemCategoryListPC@CTCInventory@@QAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@
    //     V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@ABVCCharString@@4_N@Z                               0x005907AB
    // (landed; its parameters: the category title, the page list, the category's (def index, count) pairs, the owning parent
    // component, the ring-picture template name, the menu-guide text symbol and whether the entries check a usable target).
    NUISystem::CComponent* GetItemsListUIComponentPC(NUISystem::CComponent* parent);                 // 0x00590F0B
    void ConstructItemCategoryListPC(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                     NUISystem::CComponent* owner, const CCharString& templateName,
                                     const CCharString& descriptionSymbol, bool checkUsable);        // 0x005907AB
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_CONSOLEMENUS
    // Nested opt-in (chunk 1007, rows 0x00590235 / 0x0058F016; needs FABLE_RETAIL_CTCINVENTORY_QUICKACCESS for the vector include):
    // the console twins of the PCMENUS pair, FableWin.pdb / Ego_r.pdb spellings
    //   ?GetItemsListUIComponent@CTCInventory@@QAEPAVCComponent@NUISystem@@PAV23@@Z                                0x00590235
    //   ?ConstructItemCategoryList@CTCInventory@@QAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@
    //     V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@ABVCCharString@@4_N@Z                               0x0058F016
    // (ConstructScreen 0x005907A6 is the 5-byte `jmp` to the first).
    NUISystem::CComponent* GetItemsListUIComponent(NUISystem::CComponent* parent);                   // 0x00590235
    void ConstructItemCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                   NUISystem::CComponent* owner, const CCharString& templateName,
                                   const CCharString& descriptionSymbol, bool checkUsable);          // 0x0058F016
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
    // Nested opt-in (chunk 867, row 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList): the quick-access slots and the
    // two PC menu maps as typed containers (Ego_r.pdb types; retail reads QuickAccessItems begin/end +0x148/+0x14c and clears the
    // trees at +0x1fc / +0x208 through STLport's out-of-line _Rb_tree::clear 0x00593235), the interface id (ETCInterfaceType 17,
    // the key GetTC looks up: `push 11h`) and ?GetQuickAccessItemGraphicIndex@CTCInventory@@QBEJJ@Z 0x00581FA4 (FableWin.pdb /
    // Ego_r.pdb; out of line in retail).
    static ETCInterfaceType StatGetInterfaceType() { return TCI_INVENTORY; }
    long GetQuickAccessItemGraphicIndex(long slot) const;            // 0x00581FA4
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_ITEMSELECTED
    // Nested opt-in (chunk 1007, row 0x00583E26): the slot-33 override ?OnItemSelected@CTCInventory@@UAEXJAA_N@Z (Ego_r.pdb /
    // FableWin.pdb; CTCInventory vtable 0x01250DE4 slot 33). Needs FABLE_RETAIL_CTCINVENTORYBASE_VTABLE.
    virtual void OnItemSelected(long itemDefIndex, bool& handled);   // 0x00583E26
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_OPENPC
    // Nested opt-in (chunk 1007, rows 0x0058F649 OpenPCInventory / 0x0058F9BC OpenPCPhotojournal): Ego_r.pdb / FableWin.pdb
    // ?OpenPCInventory@CTCInventory@@QAEXXZ, ?OpenPCPhotojournal@CTCInventory@@QAEXXZ and ?HasScreenshots@CTCInventory@@QAE_NXZ
    // (landed 0x00581B33, out of line).
    void OpenPCInventory();                                          // 0x0058F649
    void OpenPCPhotojournal();                                       // 0x0058F9BC
    bool HasScreenshots();                                           // 0x00581B33
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_ITEMENTRY
    // Nested opt-in (chunk 1007, rows 0x0058A0C7 / 0x0058E208): Ego_r.pdb / FableWin.pdb
    // ?ConstructItemEntry@CTCInventory@@QAEXJJPAVCList@NUISystem@@PAVCComponent@3@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z and
    // ?ConstructItemEntryPC@CTCInventory@@ (same parameters), the console / PC item menu-entry builders. Needs QUICKACCESS (the maps).
    void ConstructItemEntry(long defIndex, long count, NUISystem::CList* list, NUISystem::CComponent* owner,
                            CCountedPointer<NUISystem::CComponent>& actionsList);           // 0x0058A0C7
    void ConstructItemEntryPC(long defIndex, long count, NUISystem::CList* list, NUISystem::CComponent* owner,
                              CCountedPointer<NUISystem::CComponent>& actionsList);         // 0x0058E208
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_ASSIGNTHINGPC
    // Nested opt-in (chunk Q24, row 0x00584B86): the PC quick-access slot assignment, Ego_r.pdb / FableWin.pdb spellings
    //   ?AssignThingPC@CTCInventory@@QAEXJKPAVCComponent@NUISystem@@@Z                                             0x00584B86
    //   ?SetThingAsQuickAccessItem@CTCInventory@@QAEXJJ_N@Z                                                      0x00584157
    // (the second out of line in retail: `push 1 ; push slot ; push thing ; call 0x00584157`). Declarations only.
    void SetThingAsQuickAccessItem(long thingIndex, long slot, bool assign);                     // 0x00584157
    void AssignThingPC(long thingIndex, unsigned long slot, NUISystem::CComponent* container);    // 0x00584B86
#endif
    void OpenInventoryShortcut(long screen);
    void DeleteMenuComponents();                                     // 0x00587A41
    void OpenPCInventoryMenu();                                      // 0x0058807D
    void OpenPCMapMenu();                                            // 0x00589A30
    void OpenPCSkillsMenu();                                         // 0x00587BFF
    void OpenPCStatusMenu();                                         // 0x0058A763
#ifdef FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
    // Nested opt-in (chunk 880, row 0x0058AF10): ?OpenPCOptionsMenu@CTCInventory@@QAEXXZ (Ego_r.pdb / FableWin.pdb, public), the PC
    // options screen builder. The open-menu id is the PDB's nested enum CTCInventory::CURRENT_OPEN_MENU (pdb_class_keys.tsv W4,
    // ?GetCurrentOpenMenu@CTCInventory@@QAE?AW4CURRENT_OPEN_MENU@1@XZ); its enumerator names are not in the name sets, so only the
    // type is declared and the row stores its value (4, `mov dword ptr [ebx+21Ch],4`) by cast.
    enum CURRENT_OPEN_MENU { };
    void OpenPCOptionsMenu();
#endif
    long          ThingToAssign;                             // +0x134
    long          ThingSlotToSwap;                           // +0x138
    NUISystem::CComponent* PComponentSlotToSwap;                      // +0x13c
    long          ScreenToShortcut;                          // +0x140
    unsigned char EditBoxes[0x4];                            // +0x144 list<NUISystem::CEditBox_*,std::allocator<NUISystem::CEditBox_*>_>
#ifdef FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
    std::vector<long> QuickAccessItems;                      // +0x148
#else
    unsigned char QuickAccessItems[0xc];                     // +0x148 vector<long,std::allocator<long>_>
#endif
    unsigned char ContextSensitiveItems[0xc];                // +0x154 vector<long,std::allocator<long>_>
    unsigned char ContextSensitiveFadeTimes[0xc];            // +0x160 vector<CFadeInFadeOutBase,std::allocator<CFadeInFadeOutBase>_>
    unsigned char InventoryMainPrimitiveHandles[0xc];        // +0x16c vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char QuickAccessPrimitiveHandles[0xc];          // +0x178 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char QuickAccessUnusableIconPrimitiveHandles[0xc];  // +0x184 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char OldObjectRHSet[0x18];                      // +0x190 CRightHandedSet
    float         OldRotationAngle;                          // +0x1a8
    unsigned char CurrentObjectRHSet[0x18];                  // +0x1ac CRightHandedSet
    float         CurrentRotationAngle;                      // +0x1c4
    long          ItemRHSetsApplyTo;                         // +0x1c8
    bool          EnableQuickAccessAssign;                   // +0x1cc
    bool          EnableQuickAccessRemove;                   // +0x1cd
    bool          QuickAccessFlashState;                     // +0x1ce
    unsigned char _pad_0x1cf[0x1];                           // +0x1cf
    long          QuickAccessFlashTimer;                     // +0x1d0
    bool          ContextSensitiveUpdate;                    // +0x1d4
    unsigned char _pad_0x1d5[0x1];                           // +0x1d5
    unsigned char _pad_0x1d6[0x2];                           // +0x1d6
    unsigned char NumberOfCSOpinionExpressions[0xc];         // +0x1d8 vector<long,std::allocator<long>_>
    unsigned char LastCSOpinionExpression[0xc];              // +0x1e4 vector<long,std::allocator<long>_>
    unsigned char LastButOneCSOpinionExpression[0xc];        // +0x1f0 vector<long,std::allocator<long>_>
#ifdef FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
    std::map<long, NUISystem::CComponent*> ItemsMenuEntries; // +0x1fc
    std::map<long, NUISystem::CComponent*> AssignedIcons;    // +0x208
#else
    unsigned char ItemsMenuEntries[0xc];                     // +0x1fc map<long,NUISystem::CComponent_*,std::less<long>,std::allocator<std::pair<long_const_,NUISystem::CComponent_*>_>_>
    unsigned char AssignedIcons[0xc];                        // +0x208 map<long,NUISystem::CComponent_*,std::less<long>,std::allocator<std::pair<long_const_,NUISystem::CComponent_*>_>_>
#endif
    NUISystem::CComponent* PMouse_Pointer;                            // +0x214
    NUISystem::CComponent* PTop_Level_List;                           // +0x218
#ifdef FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
    CURRENT_OPEN_MENU CurrentOpenMenu;                                // +0x21c (typed, same 4 bytes)
#else
    unsigned char CurrentOpenMenu[0x4];                      // +0x21c CURRENT_OPEN_MENU
#endif
    float         AccumFadeTime;                             // +0x220
    unsigned char _pad_0x224[0x4];                           // +0x224
#ifdef FABLE_RETAIL_CTCINVENTORY_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x00594BE4
public:
    virtual void OnChangeCategory();  // slot 37, 0x00594BE7
public:
#endif
#ifdef FABLE_RETAIL_CTCINVENTORY_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual override this declaration lacks, declaration only (no layout change; the vtable exists). Slot from the TLC
// vtable (agentFAM03 proof, re-verified by agentFAM05); spelling from the Ego_r / FableWin name sets.
public:
    virtual ETCInterfaceType GetInterfaceType() const;  // slot 21, 0x00594BC9
public:
#endif // FABLE_RETAIL_CTCINVENTORY_VSLOTS3
#ifdef FABLE_RETAIL_CTCINVENTORY_HISCREEN
// agentFAM13 (2026-11-12): the NHeroInformationScreens::CBase overrides this declaration lacks (the secondary vtable at +0x130,
// slots 0 GetScreenName / 1 GetPlayerMode / 2 GetScreenType / 3 IsInventoryScreen, engine/NHeroInformationScreens_CBase.h);
// spellings from the Ego_r / FableWin name sets. Declaration only (no layout change; the vtable exists).
public:
    virtual EPlayerMode GetPlayerMode() const;
    virtual NHeroInformationScreens::EScreenType GetScreenType() const;
#endif // FABLE_RETAIL_CTCINVENTORY_HISCREEN
};
FABLE_STATIC_ASSERT(sizeof(CTCInventory) == 0x228);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, PTop_Level_List) == 0x218);
#ifdef FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
FABLE_STATIC_ASSERT(offsetof(CTCInventory, CurrentOpenMenu) == 0x21c);
#endif
FABLE_STATIC_ASSERT(offsetof(CTCInventory, QuickAccessItems) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ItemsMenuEntries) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, AssignedIcons) == 0x208);
#endif // FABLE_ENGINE_CTCINVENTORY_RETAIL_H
#else // generated flattened default (unchanged)
#ifndef FABLE_ENGINE_CTCINVENTORY_H
#define FABLE_ENGINE_CTCINVENTORY_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCInventory = 0x270.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCInventory = 0x228.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CComponent;

#pragma pack(push, 1)
class CTCInventory {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0xc];                            // +0x0c
    unsigned char _pad_0x18[0x1];                            // +0x18
    unsigned char _pad_0x19[0x3];                            // +0x19
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char _pad_0x20[0xc];                            // +0x20
    unsigned char _pad_0x2c[0x4];                            // +0x2c
    unsigned char _pad_0x30[0x4];                            // +0x30
    unsigned char _pad_0x34[0x2];                            // +0x34
    unsigned char _pad_0x36[0x2];                            // +0x36
    unsigned char _pad_0x38[0x4];                            // +0x38
    unsigned char _pad_0x3c[0x4];                            // +0x3c
    unsigned char _pad_0x40[0x4];                            // +0x40
    unsigned char _pad_0x44[0x4];                            // +0x44
    unsigned char _pad_0x48[0x4];                            // +0x48
    unsigned char _pad_0x4c[0x4];                            // +0x4c
    unsigned char _pad_0x50[0x4];                            // +0x50
    unsigned char _pad_0x54[0x4];                            // +0x54
    unsigned char _pad_0x58[0x1];                            // +0x58
    unsigned char _pad_0x59[0x3];                            // +0x59
    unsigned char _pad_0x5c[0x4];                            // +0x5c
    unsigned char _pad_0x60[0x4];                            // +0x60
    unsigned char _pad_0x64[0x1];                            // +0x64
    unsigned char _pad_0x65[0x3];                            // +0x65
    unsigned char _pad_0x68[0x4];                            // +0x68
    unsigned char _pad_0x6c[0x4];                            // +0x6c
    unsigned char _pad_0x70[0x4];                            // +0x70
    unsigned char _pad_0x74[0x4];                            // +0x74
    unsigned char _pad_0x78[0x4];                            // +0x78
    unsigned char _pad_0x7c[0x4];                            // +0x7c
    unsigned char _pad_0x80[0x4];                            // +0x80
    unsigned char _pad_0x84[0x4];                            // +0x84
    unsigned char _pad_0x88[0x2];                            // +0x88
    unsigned char _pad_0x8a[0x2];                            // +0x8a
    unsigned char _pad_0x8c[0x4];                            // +0x8c
    unsigned char _pad_0x90[0x4];                            // +0x90
    unsigned char _pad_0x94[0x24];                           // +0x94
    unsigned char _pad_0xb8[0x4];                            // +0xb8
    unsigned char _pad_0xbc[0x4];                            // +0xbc
    unsigned char _pad_0xc0[0x4];                            // +0xc0
    unsigned char _pad_0xc4[0x4];                            // +0xc4
    unsigned char _pad_0xc8[0x4];                            // +0xc8
    unsigned char _pad_0xcc[0x4];                            // +0xcc
    unsigned char _pad_0xd0[0x4];                            // +0xd0
    unsigned char _pad_0xd4[0x4];                            // +0xd4
    unsigned char _pad_0xd8[0x4];                            // +0xd8
    unsigned char _pad_0xdc[0x4];                            // +0xdc
    unsigned char _pad_0xe0[0x4];                            // +0xe0
    unsigned char _pad_0xe4[0x4];                            // +0xe4
    unsigned char _pad_0xe8[0x4];                            // +0xe8
    unsigned char _pad_0xec[0x4];                            // +0xec
    unsigned char _pad_0xf0[0x4];                            // +0xf0
    unsigned char _pad_0xf4[0x4];                            // +0xf4
    unsigned char _pad_0xf8[0x4];                            // +0xf8
    unsigned char _pad_0xfc[0x4];                            // +0xfc
    unsigned char _pad_0x100[0x4];                           // +0x100
    unsigned char _pad_0x104[0x4];                           // +0x104
    unsigned char _pad_0x108[0x4];                           // +0x108
    unsigned char _pad_0x10c[0x4];                           // +0x10c
    unsigned char _pad_0x110[0x4];                           // +0x110
    unsigned char _pad_0x114[0x4];                           // +0x114
    unsigned char _pad_0x118[0x4];                           // +0x118
    unsigned char _pad_0x11c[0x4];                           // +0x11c
    unsigned char _pad_0x120[0x2];                           // +0x120
    unsigned char _pad_0x122[0x2];                           // +0x122
    unsigned char _pad_0x124[0x4];                           // +0x124
    unsigned char _pad_0x128[0x4];                           // +0x128
    unsigned char _pad_0x12c[0x4];                           // +0x12c
    unsigned char _pad_0x130[0x4];                           // +0x130
    long          ThingToAssign;                             // +0x134
    long          ThingSlotToSwap;                           // +0x138
    CComponent*   PComponentSlotToSwap;                      // +0x13c
    long          ScreenToShortcut;                          // +0x140
    unsigned char EditBoxes[0x4];                            // +0x144 list<NUISystem::CEditBox_*,std::allocator<NUISystem::CEditBox_*>_>
    unsigned char QuickAccessItems[0xc];                     // +0x148 vector<long,std::allocator<long>_>
    unsigned char ContextSensitiveItems[0xc];                // +0x154 vector<long,std::allocator<long>_>
    unsigned char ContextSensitiveFadeTimes[0xc];            // +0x160 vector<CFadeInFadeOutBase,std::allocator<CFadeInFadeOutBase>_>
    unsigned char InventoryMainPrimitiveHandles[0xc];        // +0x16c vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char QuickAccessPrimitiveHandles[0xc];          // +0x178 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char QuickAccessUnusableIconPrimitiveHandles[0xc];  // +0x184 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char OldObjectRHSet[0x18];                      // +0x190 CRightHandedSet
    float         OldRotationAngle;                          // +0x1a8
    unsigned char CurrentObjectRHSet[0x18];                  // +0x1ac CRightHandedSet
    float         CurrentRotationAngle;                      // +0x1c4
    long          ItemRHSetsApplyTo;                         // +0x1c8
    bool          EnableQuickAccessAssign;                   // +0x1cc
    bool          EnableQuickAccessRemove;                   // +0x1cd
    bool          QuickAccessFlashState;                     // +0x1ce
    unsigned char _pad_0x1cf[0x1];                           // +0x1cf
    long          QuickAccessFlashTimer;                     // +0x1d0
    bool          ContextSensitiveUpdate;                    // +0x1d4
    unsigned char _pad_0x1d5[0x1];                           // +0x1d5
    unsigned char _pad_0x1d6[0x2];                           // +0x1d6
    unsigned char NumberOfCSOpinionExpressions[0xc];         // +0x1d8 vector<long,std::allocator<long>_>
    unsigned char LastCSOpinionExpression[0xc];              // +0x1e4 vector<long,std::allocator<long>_>
    unsigned char LastButOneCSOpinionExpression[0xc];        // +0x1f0 vector<long,std::allocator<long>_>
    unsigned char ItemsMenuEntries[0xc];                     // +0x1fc map<long,NUISystem::CComponent_*,std::less<long>,std::allocator<std::pair<long_const_,NUISystem::CComponent_*>_>_>
    unsigned char AssignedIcons[0xc];                        // +0x208 map<long,NUISystem::CComponent_*,std::less<long>,std::allocator<std::pair<long_const_,NUISystem::CComponent_*>_>_>
    CComponent*   PMouse_Pointer;                            // +0x214
    CComponent*   PTop_Level_List;                           // +0x218
    unsigned char CurrentOpenMenu[0x4];                      // +0x21c CURRENT_OPEN_MENU
    float         AccumFadeTime;                             // +0x220
    unsigned char _pad_0x224[0x4];                           // +0x224
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCInventory) == 0x228);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ThingToAssign) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ThingSlotToSwap) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, PComponentSlotToSwap) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ScreenToShortcut) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, EditBoxes) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, QuickAccessItems) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ContextSensitiveItems) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ContextSensitiveFadeTimes) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, InventoryMainPrimitiveHandles) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, QuickAccessPrimitiveHandles) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, QuickAccessUnusableIconPrimitiveHandles) == 0x184);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, OldObjectRHSet) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, OldRotationAngle) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, CurrentObjectRHSet) == 0x1ac);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, CurrentRotationAngle) == 0x1c4);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ItemRHSetsApplyTo) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, EnableQuickAccessAssign) == 0x1cc);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, EnableQuickAccessRemove) == 0x1cd);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, QuickAccessFlashState) == 0x1ce);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, QuickAccessFlashTimer) == 0x1d0);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ContextSensitiveUpdate) == 0x1d4);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, NumberOfCSOpinionExpressions) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, LastCSOpinionExpression) == 0x1e4);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, LastButOneCSOpinionExpression) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, ItemsMenuEntries) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, AssignedIcons) == 0x208);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, PMouse_Pointer) == 0x214);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, PTop_Level_List) == 0x218);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, CurrentOpenMenu) == 0x21c);
FABLE_STATIC_ASSERT(offsetof(CTCInventory, AccumFadeTime) == 0x220);

#endif // FABLE_ENGINE_CTCINVENTORY_H
#endif // FABLE_RETAIL_CTCINVENTORY

#endif
