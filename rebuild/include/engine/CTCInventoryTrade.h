#ifdef FABLE_RETAIL_CTCINVENTORYTRADE
#ifndef FABLE_ENGINE_CTCINVENTORYTRADE_RETAIL_H
#define FABLE_ENGINE_CTCINVENTORYTRADE_RETAIL_H
// Retail opt-in (chunk 867, 2026-11-08, rows 0x005A8C66 ConstructActualSpecialInventoryPC / 0x005AF64E BuyItem / 0x005B01FE SellItemPC):
// the class with its two RTTI bases (.?AVCTCInventoryTrade@@: NInventory::CTCInventoryBase at +0, sizeof 0x130, and
// NHeroInformationScreens::CBase at +0x130, as engine/CTCInventory.h's retail branch) and the Ego_r.pdb members typed at the retail
// STLport 4.6 offsets (list 4, map 0xC; the landed constructor 0x0059EC48 / destructor model: TradeAnimPrimitiveHandles +0x134 ..
// AccumFadeTime +0x228, sizeof 0x230, matching the generated layout below). Methods: Ego_r.pdb + FableWin.pdb spellings, all out of
// line in retail unless noted.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/NHeroInformationScreens_CBase.h"
#include "engine/CEnginePrimitiveHandle.h"
namespace NUISystem { class CComponent; class CList; class CViewport; }
class CHairCardDef;
class CTCInventoryTrade : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    enum EInventoryCategoryIdentifier { INVENTORY_CATEGORY_UNKNOWN = 0 };   // W4EInventoryCategoryIdentifier@1@ (enumerators unproven)
    // W4EItem_Menu_Type@1@ (GetTypeFromDefIndexPC / GetMenuBranchFromDefIndexPC): 0 clothes, 1 weapons, 2 items -- the switch arms of
    // 0x005B01FE SellItemPC / 0x005AF64E BuyItem add to TotNumClothes.. / TotNumWeapons.. / TotNumItems.. in that order; the landed
    // GetTypeFromDefIndexPC 0x0059C447 returns 1 for the weapon categories, 0 for the clothing ones, 2 otherwise. Enumerator spellings unproven.
    enum EItem_Menu_Type { ITEM_MENU_TYPE_CLOTHES = 0, ITEM_MENU_TYPE_WEAPONS = 1, ITEM_MENU_TYPE_ITEMS = 2 };
    void ResetTradeLists();                                                        // 0x0059D9AD ?ResetTradeLists@CTCInventoryTrade@@QAEXXZ
    void InitialiseBuyableItems(std::vector<std::pair<long,long> >& items);        // 0x005A30A1
    bool AddBuyItemToListPC(NUISystem::CComponent* frame, long defIndex, long count, NUISystem::CList* list,
                            NUISystem::CViewport* viewport, bool a);               // 0x005A6CA1
    void BuyItem(NUISystem::CComponent* entry, long count);                        // 0x005AF64E
    void SellItemPC(NUISystem::CComponent* entry, long count);                     // 0x005B01FE
    bool IsSpecialCase(long itemDefIndex);                                         // 0x0059C983 ?IsSpecialCase@CTCInventoryTrade@@QAE_NJ@Z
    void RefreshNameAndTitlePC(NUISystem::CComponent* entry, long itemDefIndex, long count);   // 0x0059CA95
    void RefreshNameAndTitle(NUISystem::CComponent* entry, long itemDefIndex, long count);     // 0x0059CFBD
    void RefreshSellDescriptionsPC(NUISystem::CComponent* parent);                 // 0x005AEAEE
    // Row 0x005AF64E BuyItem (the console buy path), FableWin.pdb / Ego_r.pdb spellings; all out of line in retail:
    bool AddSellItemToList(long itemDefIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool a);   // 0x0059ED42 QAE_NJJPAVCList@NUISystem@@PAVCViewport@3@_N@Z
    void ApplyHairCard(const CHairCardDef& hairCard);                              // 0x0059D9C2 QAEXABVCHairCardDef@@@Z
    void ApplyTattooCard(long itemDefIndex);                                       // 0x005AD305 QAEXJ@Z
    void RefreshDescriptions();                                                    // 0x005AE6AF QAEXXZ
#ifdef FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
    virtual void RemoveItemFromInventory(long itemDefIndex, long count);           // slot 79 0x0059C747 (UAEXJJ@Z override)
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_OPENINVENTORY
    // Opt-in (agentSND, 2026-11-10, row 0x005A94BD): Ego_r.pdb / FableWin.pdb ?OpenInventory@CTCInventoryTrade@@UAEXXZ, the override in
    // CTCInventoryTrade vtable 0x01253034 slot 30 (slot 31 CloseInventory 0x0059F304, slot 86 OpenInventoryPC 0x005AE86B).
    virtual void OpenInventory();                                                  // slot 30 0x005A94BD
#endif
private:
    void ConstructActualSpecialInventoryPC();                                      // 0x005A8C66 (private: AAEXXZ)
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_ACTUALINVENTORYPC
    // Opt-in (agentSND, 2026-11-10, row 0x005ACEDA): FableWin.pdb ?ConstructActualInventoryPC@CTCInventoryTrade@@AAEXXZ (private), out of
    // line at 0x005ACEDA; its only caller is OpenInventoryPC 0x005AE86B (chunk 1009 NOTES: the landed caller's ConstructTradeScreenPC).
    void ConstructActualInventoryPC();                                             // 0x005ACEDA
#endif
    // The private PC helpers (Ego_r.pdb AAE..): the menu branch (category entry) an item def belongs to in the open trade menu, with
    // its EItem_Menu_Type through the reference; the item's EItem_Menu_Type; the per-category subtitle refresh; the sell-actions list
    // rebuild; the entry amounts / availabilities refresh (its pair<ulong,ulong> result is the (amount, available) totals).
    NUISystem::CComponent* GetMenuBranchFromDefIndexPC(long itemDefIndex, NUISystem::CComponent* menu, EItem_Menu_Type& type);   // 0x0059C0C6
    EItem_Menu_Type GetTypeFromDefIndexPC(long itemDefIndex);                      // 0x0059C447
    void RefreshSubtitlesPC(NUISystem::CComponent* menu, std::map<NUISystem::CComponent*,std::pair<CWideString,long> >& subtitles);   // 0x0059E94D
    void UpdateSellActionsList();                                                  // 0x0059DE76
    // Row 0x005AF64E BuyItem, the private console-path helpers (Ego_r.pdb / FableWin.pdb AAE..): the menu branch of an item def in
    // the console trading menu with its EItem_Menu_Type through the reference (0x0059B79A), whether a special item is already owned
    // (0x0059FC17), the per-category subtitle refresh (0x0059E108), the buy-actions list rebuild (0x0059DE04), the entry amounts /
    // availabilities refresh (0x0059DB82) and the buy-possibility refresh (0x005A9A87: neither PDB has its address; it is the only
    // private void() method of the class without a retail address, and the manifest names 0x005A887D, its PC twin by the Ego_r
    // donor order, "PC_PopulateBuyList" and this one "Inventory_PopulateBuyList").
    NUISystem::CComponent* GetMenuBranchFromDefIndex(long itemDefIndex, NUISystem::CComponent* menu, EItem_Menu_Type& type);   // 0x0059B79A
    bool AlreadyHaveSpecial(long itemDefIndex);                                    // 0x0059FC17
    void RefreshSubtitles(NUISystem::CComponent* menu, std::map<NUISystem::CComponent*,std::pair<CWideString,long> >& subtitles);   // 0x0059E108
    void UpdateBuyActionsList();                                                   // 0x0059DE04
    std::pair<unsigned long,unsigned long> RefreshEntriesAmountsAndAvailabilities(NUISystem::CComponent* menu,
                                            std::map<NUISystem::CComponent*,NUISystem::CList*>& lists);   // 0x0059DB82
    void RefreshBuyPossibility();                                                  // 0x005A9A87
    std::pair<unsigned long,unsigned long> RefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* menu,
                                            std::map<NUISystem::CComponent*,NUISystem::CList*>& lists);   // 0x0059FAFE
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
    // Retail opt-in (chunk 891, rows 0x005A4CDD ConstructActualSellInventoryPC / 0x005A5B31 ConstructActualWantedInventoryPC /
    // 0x005AC10E ConstructActualBuyInventoryPC): the three PC inventory builders (Ego_r.pdb + FableWin.pdb private AAEXXZ) and the
    // helpers they call, in Ego_r.pdb spelling, all out of line in retail (declarations only, no layout change):
    // ?AttachToFrame@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@@Z 0x0059BB1B,
    // ?AddWantedItemToListPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@JPAVCList@3@PAVCViewport@3@@Z 0x0059D270 (`ret 10h`),
    // ?AddSellItemToListPC@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJPAVCList@3@PAVCViewport@3@_N@Z 0x0059F51E,
    // ?InitialiseSellableItems@CTCInventoryTrade@@QAEXXZ 0x0059D820,
    // ?ConstructSubtitlesPC@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@0@Z 0x0059E4D7.
public:
    void AttachToFrame(NUISystem::CComponent* component);                        // 0x0059BB1B
    void AddWantedItemToListPC(NUISystem::CComponent* frame, long itemDefIndex, NUISystem::CList* list,
                               NUISystem::CViewport* viewport);                    // 0x0059D270
    bool AddSellItemToListPC(NUISystem::CComponent* frame, long itemDefIndex, long count, NUISystem::CList* list,
                             NUISystem::CViewport* viewport, bool addToTop);       // 0x0059F51E
    void InitialiseSellableItems();                                                // 0x0059D820
private:
    void ConstructSubtitlesPC(NUISystem::CComponent* menu, std::map<NUISystem::CComponent*,std::pair<CWideString,long> >& subtitles,
                              NUISystem::CComponent* frame);                       // 0x0059E4D7
    void ConstructActualSellInventoryPC();                                         // 0x005A4CDD
    void ConstructActualWantedInventoryPC();                                       // 0x005A5B31
    void ConstructActualBuyInventoryPC();                                          // 0x005AC10E
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_WANTEDLIST
    // Retail opt-in (chunk 1013, re-author of row 0x0059CB90): Ego_r.pdb / FableWin.pdb
    // ?AddWantedItemToList@CTCInventoryTrade@@QAEXJPAVCList@NUISystem@@PAVCViewport@3@@Z (`ret 0Ch`), the console twin of
    // AddWantedItemToListPC 0x0059D270. Public, declaration only.
public:
    void AddWantedItemToList(long itemDefIndex, NUISystem::CList* list, NUISystem::CViewport* viewport);   // 0x0059CB90
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC
    // Retail opt-in (chunk 892, row 0x005A887D): Ego_r.pdb / FableWin.pdb ?RefreshBuyPossibilityPC@CTCInventoryTrade@@AAEXXZ (the row;
    // the manifest's "PC_PopulateBuyList" is descriptive) and ?RefreshBuyPossibilityNumbersPC@CTCInventoryTrade@@AAEXXZ (0x005A8175,
    // out of line). Private, declarations only.
private:
    void RefreshBuyPossibilityNumbersPC();                                         // 0x005A8175
    void RefreshBuyPossibilityPC();                                                // 0x005A887D
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_BUYITEMPC
    // Retail opt-in (chunk 1009, row 0x005B0AAC BuyItemPC): FableWin.pdb ?BuyItemPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z
    // (the row) and ?RefreshBuyDescriptionsPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@@Z (0x005AE9DE, landed, out of line).
    // Public, declarations only.
public:
    void BuyItemPC(NUISystem::CComponent* entry, long count);                      // 0x005B0AAC
    void RefreshBuyDescriptionsPC(NUISystem::CComponent* parent);                  // 0x005AE9DE
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_ACTIONSLISTS
    // Retail opt-in (chunk 1009b, rows 0x005A3E47 ConstructBuyActionsList / 0x005A79E6 ConstructBuyActionsListPC): Ego_r.pdb +
    // FableWin.pdb ?ConstructBuyActionsList@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z (the rows; the PC form
    // ?ConstructBuyActionsListPC@..), the sell twins ?ConstructSellActionsList(PC)@.. (0x005A45F6 / 0x005A731F, same signature) and
    // ?IsItemAffordable@CTCInventoryTrade@@QAE_NJJ@Z (0x0059DA10). Public, all out of line in retail; declarations only.
public:
    bool IsItemAffordable(long itemDefIndex, long count);                          // 0x0059DA10
    bool ConstructBuyActionsList(NUISystem::CComponent* parent, long itemDefIndex, long count, bool dynamically);     // 0x005A3E47
    bool ConstructSellActionsList(NUISystem::CComponent* parent, long itemDefIndex, long count, bool dynamically);    // 0x005A45F6
    bool ConstructSellActionsListPC(NUISystem::CComponent* parent, long itemDefIndex, long count, bool dynamically);  // 0x005A731F
    bool ConstructBuyActionsListPC(NUISystem::CComponent* parent, long itemDefIndex, long count, bool dynamically);   // 0x005A79E6
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_SHOPLISTS
    // Retail opt-in (chunk 1009b, rows 0x005A6769 ConstructNormalShopListPC / 0x005A29F4 ConstructSpecialShopList / 0x0059FEE3
    // AddBuyItemToList): Ego_r.pdb + FableWin.pdb, public, all out of line in retail (declarations only):
    // ?ConstructNormalShopList@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@ABV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@0@Z
    // and its PC form (0x005A6769), ?ConstructSpecialShopList@CTCInventoryTrade@@QAEPAVCComponent@NUISystem@@ABV?$vector@..@std@@AAVCCharString@@@Z
    // (0x005A29F4) and its PC form (0x005A8A5D), ?AddBuyItemToList@CTCInventoryTrade@@QAE_NJJPAVCList@NUISystem@@PAVCViewport@3@_N@Z
    // (0x0059FEE3; the PC form AddBuyItemToListPC 0x005A6CA1 is declared above).
public:
    NUISystem::CComponent* ConstructNormalShopList(const std::vector<std::pair<long,long> >& buyItems,
                                                   const std::vector<std::pair<long,long> >& sellItems);
    NUISystem::CComponent* ConstructNormalShopListPC(const std::vector<std::pair<long,long> >& buyItems,
                                                     const std::vector<std::pair<long,long> >& sellItems);     // 0x005A6769
    NUISystem::CComponent* ConstructSpecialShopList(const std::vector<std::pair<long,long> >& buyItems, CCharString& name);     // 0x005A29F4
    NUISystem::CComponent* ConstructSpecialShopListPC(const std::vector<std::pair<long,long> >& buyItems, CCharString& name);   // 0x005A8A5D
    bool AddBuyItemToList(long itemDefIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool addToTop);   // 0x0059FEE3
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITY
    // Retail opt-in (chunk 1009, row 0x005A9A87 RefreshBuyPossibility): Ego_r.pdb / FableWin.pdb
    // ?RefreshBuyPossibilityNumbers@CTCInventoryTrade@@AAEXXZ (0x005A3734, out of line; the console twin of
    // RefreshBuyPossibilityNumbersPC 0x005A8175). Private, declaration only.
private:
    void RefreshBuyPossibilityNumbers();                                           // 0x005A3734
#endif
public:
    std::vector<CEnginePrimitiveHandle> TradeAnimPrimitiveHandles;                 // +0x134
    std::list<long> NotInStock;                                                    // +0x140
    std::map<NUISystem::CComponent*,std::pair<long,long> > ItemsToBuy;             // +0x144
    std::map<NUISystem::CComponent*,std::pair<long,long> > ItemsToSell;            // +0x150
    bool IsShoppingValid;                                                          // +0x15c
    long CategoryCount;                                                            // +0x160
    long ItemSellCount;                                                            // +0x164
    long ItemBuyCount;                                                             // +0x168
    long ItemSellUniqueCount;                                                      // +0x16c
    EInventoryCategoryIdentifier CategoryIdent;                                    // +0x170
    NHeroInformationScreens::EScreenType PreviousScreenType;                       // +0x174
    bool WarningNeeded;                                                            // +0x178
    long TrophyUseNeeded;                                                          // +0x17c
    std::map<NUISystem::CComponent*,NUISystem::CList*> SellListsByCategory;        // +0x180
    std::map<NUISystem::CComponent*,NUISystem::CList*> BuyListsByCategory;         // +0x18c
    std::map<NUISystem::CComponent*,NUISystem::CList*> WantedListsByCategory;      // +0x198
    std::map<long,long> SpecialItemsToDelete;                                      // +0x1a4
    std::map<NUISystem::CComponent*,std::pair<CWideString,long> > NumBuyableThings;    // +0x1b0
    std::map<NUISystem::CComponent*,std::pair<CWideString,long> > NumSellableThings;   // +0x1bc
    std::map<NUISystem::CComponent*,std::pair<CWideString,long> > NumWantedThings;     // +0x1c8
    long TotNumClothesToSell;                                                      // +0x1d4
    long TotNumWeaponsToSell;                                                      // +0x1d8
    long TotNumItemsToSell;                                                        // +0x1dc
    long TotNumClothesToBuy;                                                       // +0x1e0
    long TotNumWeaponsToBuy;                                                       // +0x1e4
    long TotNumItemsToBuy;                                                         // +0x1e8
    long TotNumClothesWanted;                                                      // +0x1ec
    long TotNumWeaponsWanted;                                                      // +0x1f0
    long TotNumItemsWanted;                                                        // +0x1f4
    long TotSpecialItemsToBuy;                                                     // +0x1f8
    CWideString SpecialItemString;                                                 // +0x1fc
    std::map<NUISystem::CComponent*,NUISystem::CComponent*> ItemBuyLists;          // +0x200
    std::map<NUISystem::CComponent*,NUISystem::CComponent*> ItemSellLists;         // +0x20c
    bool InventoryTradeOpen;                                                       // +0x218
    bool InventoryTradeInitialised;                                                // +0x219
    NUISystem::CComponent* PMouse_Pointer;                                         // +0x21c
    NUISystem::CComponent* PTop_Level_List;                                        // +0x220
    NUISystem::CComponent* PCurrentOpenTradeMenu;                                  // +0x224
    float AccumFadeTime;                                                           // +0x228
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x005B3070
public:
    virtual void OnChangeCategory();  // slot 37, 0x005B306F
public:
#endif
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual overrides this declaration lacks, declaration only (no layout change; the vtable exists). Slots from the TLC
// vtable (agentFAM03 proof, re-verified by agentFAM05); spellings from the Ego_r / FableWin name sets.
public:
    virtual ETCInterfaceType GetInterfaceType() const;  // slot 21, 0x005B304A
public:
    virtual bool GetInventoryShoppingValidity();  // slot 48, 0x0059B6E8
public:
#endif // FABLE_RETAIL_CTCINVENTORYTRADE_VSLOTS3
#ifdef FABLE_RETAIL_CTCINVENTORYTRADE_HISCREEN
// agentFAM13 (2026-11-12): the NHeroInformationScreens::CBase overrides this declaration lacks (the secondary vtable at +0x130,
// slots 0 GetScreenName / 1 GetPlayerMode / 2 GetScreenType / 3 IsInventoryScreen, engine/NHeroInformationScreens_CBase.h);
// spellings from the Ego_r / FableWin name sets. Declaration only (no layout change; the vtable exists).
public:
    virtual EPlayerMode GetPlayerMode() const;
    virtual NHeroInformationScreens::EScreenType GetScreenType() const;
    virtual bool IsInventoryScreen() const;
#endif // FABLE_RETAIL_CTCINVENTORYTRADE_HISCREEN
};
FABLE_STATIC_ASSERT(sizeof(CTCInventoryTrade) == 0x230);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, NotInStock) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemsToBuy) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, BuyListsByCategory) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotSpecialItemsToBuy) == 0x1f8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, SpecialItemString) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemBuyLists) == 0x200);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, PTop_Level_List) == 0x220);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, PCurrentOpenTradeMenu) == 0x224);
#endif // FABLE_ENGINE_CTCINVENTORYTRADE_RETAIL_H
#else // generated flattened default (unchanged)
#ifndef FABLE_ENGINE_CTCINVENTORYTRADE_H
#define FABLE_ENGINE_CTCINVENTORYTRADE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCInventoryTrade = 0x280.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCInventoryTrade = 0x230.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CComponent;
class CWideStringData;

#pragma pack(push, 1)
class CTCInventoryTrade {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    unsigned char    _pad_0x0c[0xc];                         // +0x0c
    unsigned char    _pad_0x18[0x1];                         // +0x18
    unsigned char    _pad_0x19[0x3];                         // +0x19
    unsigned char    _pad_0x1c[0x4];                         // +0x1c
    unsigned char    _pad_0x20[0xc];                         // +0x20
    unsigned char    _pad_0x2c[0x4];                         // +0x2c
    unsigned char    _pad_0x30[0x4];                         // +0x30
    unsigned char    _pad_0x34[0x2];                         // +0x34
    unsigned char    _pad_0x36[0x2];                         // +0x36
    unsigned char    _pad_0x38[0x4];                         // +0x38
    unsigned char    _pad_0x3c[0x4];                         // +0x3c
    unsigned char    _pad_0x40[0x4];                         // +0x40
    unsigned char    _pad_0x44[0x4];                         // +0x44
    unsigned char    _pad_0x48[0x4];                         // +0x48
    unsigned char    _pad_0x4c[0x4];                         // +0x4c
    unsigned char    _pad_0x50[0x4];                         // +0x50
    unsigned char    _pad_0x54[0x4];                         // +0x54
    unsigned char    _pad_0x58[0x1];                         // +0x58
    unsigned char    _pad_0x59[0x3];                         // +0x59
    unsigned char    _pad_0x5c[0x4];                         // +0x5c
    unsigned char    _pad_0x60[0x4];                         // +0x60
    unsigned char    _pad_0x64[0x1];                         // +0x64
    unsigned char    _pad_0x65[0x3];                         // +0x65
    unsigned char    _pad_0x68[0x4];                         // +0x68
    unsigned char    _pad_0x6c[0x4];                         // +0x6c
    unsigned char    _pad_0x70[0x4];                         // +0x70
    unsigned char    _pad_0x74[0x4];                         // +0x74
    unsigned char    _pad_0x78[0x4];                         // +0x78
    unsigned char    _pad_0x7c[0x4];                         // +0x7c
    unsigned char    _pad_0x80[0x4];                         // +0x80
    unsigned char    _pad_0x84[0x4];                         // +0x84
    unsigned char    _pad_0x88[0x2];                         // +0x88
    unsigned char    _pad_0x8a[0x2];                         // +0x8a
    unsigned char    _pad_0x8c[0x4];                         // +0x8c
    unsigned char    _pad_0x90[0x4];                         // +0x90
    unsigned char    _pad_0x94[0x24];                        // +0x94
    unsigned char    _pad_0xb8[0x4];                         // +0xb8
    unsigned char    _pad_0xbc[0x4];                         // +0xbc
    unsigned char    _pad_0xc0[0x4];                         // +0xc0
    unsigned char    _pad_0xc4[0x4];                         // +0xc4
    unsigned char    _pad_0xc8[0x4];                         // +0xc8
    unsigned char    _pad_0xcc[0x4];                         // +0xcc
    unsigned char    _pad_0xd0[0x4];                         // +0xd0
    unsigned char    _pad_0xd4[0x4];                         // +0xd4
    unsigned char    _pad_0xd8[0x4];                         // +0xd8
    unsigned char    _pad_0xdc[0x4];                         // +0xdc
    unsigned char    _pad_0xe0[0x4];                         // +0xe0
    unsigned char    _pad_0xe4[0x4];                         // +0xe4
    unsigned char    _pad_0xe8[0x4];                         // +0xe8
    unsigned char    _pad_0xec[0x4];                         // +0xec
    unsigned char    _pad_0xf0[0x4];                         // +0xf0
    unsigned char    _pad_0xf4[0x4];                         // +0xf4
    unsigned char    _pad_0xf8[0x4];                         // +0xf8
    unsigned char    _pad_0xfc[0x4];                         // +0xfc
    unsigned char    _pad_0x100[0x4];                        // +0x100
    unsigned char    _pad_0x104[0x4];                        // +0x104
    unsigned char    _pad_0x108[0x4];                        // +0x108
    unsigned char    _pad_0x10c[0x4];                        // +0x10c
    unsigned char    _pad_0x110[0x4];                        // +0x110
    unsigned char    _pad_0x114[0x4];                        // +0x114
    unsigned char    _pad_0x118[0x4];                        // +0x118
    unsigned char    _pad_0x11c[0x4];                        // +0x11c
    unsigned char    _pad_0x120[0x2];                        // +0x120
    unsigned char    _pad_0x122[0x2];                        // +0x122
    unsigned char    _pad_0x124[0x4];                        // +0x124
    unsigned char    _pad_0x128[0x4];                        // +0x128
    unsigned char    _pad_0x12c[0x4];                        // +0x12c
    unsigned char    _pad_0x130[0x4];                        // +0x130
    unsigned char    TradeAnimPrimitiveHandles[0xc];         // +0x134 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char    NotInStock[0x4];                        // +0x140 list<long,std::allocator<long>_>
    unsigned char    ItemsToBuy[0xc];                        // +0x144 map<NUISystem::CComponent_*,std::pair<long,long>,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,std::pair<long,long>_>_>_>
    unsigned char    ItemsToSell[0xc];                       // +0x150 map<NUISystem::CComponent_*,std::pair<long,long>,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,std::pair<long,long>_>_>_>
    bool             IsShoppingValid;                        // +0x15c
    unsigned char    _pad_0x15d[0x3];                        // +0x15d
    long             CategoryCount;                          // +0x160
    long             ItemSellCount;                          // +0x164
    long             ItemBuyCount;                           // +0x168
    long             ItemSellUniqueCount;                    // +0x16c
    unsigned char    CategoryIdent[0x4];                     // +0x170 EInventoryCategoryIdentifier
    unsigned char    PreviousScreenType[0x4];                // +0x174 EScreenType
    bool             WarningNeeded;                          // +0x178
    unsigned char    _pad_0x179[0x3];                        // +0x179
    long             TrophyUseNeeded;                        // +0x17c
    unsigned char    SellListsByCategory[0xc];               // +0x180 map<NUISystem::CComponent_*,NUISystem::CList_*,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,NUISystem::CList_*>_>_>
    unsigned char    BuyListsByCategory[0xc];                // +0x18c map<NUISystem::CComponent_*,NUISystem::CList_*,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,NUISystem::CList_*>_>_>
    unsigned char    WantedListsByCategory[0xc];             // +0x198 map<NUISystem::CComponent_*,NUISystem::CList_*,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,NUISystem::CList_*>_>_>
    unsigned char    SpecialItemsToDelete[0xc];              // +0x1a4 map<long,long,std::less<long>,std::allocator<std::pair<long_const_,long>_>_>
    unsigned char    NumBuyableThings[0xc];                  // +0x1b0 map<NUISystem::CComponent_*,std::pair<CWideString,long>,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,std::pair<CWideString,long>_>_>_>
    unsigned char    NumSellableThings[0xc];                 // +0x1bc map<NUISystem::CComponent_*,std::pair<CWideString,long>,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,std::pair<CWideString,long>_>_>_>
    unsigned char    NumWantedThings[0xc];                   // +0x1c8 map<NUISystem::CComponent_*,std::pair<CWideString,long>,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,std::pair<CWideString,long>_>_>_>
    long             TotNumClothesToSell;                    // +0x1d4
    long             TotNumWeaponsToSell;                    // +0x1d8
    long             TotNumItemsToSell;                      // +0x1dc
    long             TotNumClothesToBuy;                     // +0x1e0
    long             TotNumWeaponsToBuy;                     // +0x1e4
    long             TotNumItemsToBuy;                       // +0x1e8
    long             TotNumClothesWanted;                    // +0x1ec
    long             TotNumWeaponsWanted;                    // +0x1f0
    long             TotNumItemsWanted;                      // +0x1f4
    long             TotSpecialItemsToBuy;                   // +0x1f8
    CWideStringData* SpecialItemString_PStringData;          // +0x1fc CWideString::PStringData
    unsigned char    ItemBuyLists[0xc];                      // +0x200 map<NUISystem::CComponent_*,NUISystem::CComponent_*,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,NUISystem::CComponent_*>_>_>
    unsigned char    ItemSellLists[0xc];                     // +0x20c map<NUISystem::CComponent_*,NUISystem::CComponent_*,std::less<NUISystem::CComponent_*>,std::allocator<std::pair<NUISystem::CComponent_*_const,NUISystem::CComponent_*>_>_>
    bool             InventoryTradeOpen;                     // +0x218
    bool             InventoryTradeInitialised;              // +0x219
    unsigned char    _pad_0x21a[0x2];                        // +0x21a
    CComponent*      PMouse_Pointer;                         // +0x21c
    CComponent*      PTop_Level_List;                        // +0x220
    CComponent*      PCurrentOpenTradeMenu;                  // +0x224
    float            AccumFadeTime;                          // +0x228
    unsigned char    _pad_0x22c[0x4];                        // +0x22c
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCInventoryTrade) == 0x230);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TradeAnimPrimitiveHandles) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, NotInStock) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemsToBuy) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemsToSell) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, IsShoppingValid) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, CategoryCount) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemSellCount) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemBuyCount) == 0x168);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemSellUniqueCount) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, CategoryIdent) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, PreviousScreenType) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, WarningNeeded) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TrophyUseNeeded) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, SellListsByCategory) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, BuyListsByCategory) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, WantedListsByCategory) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, SpecialItemsToDelete) == 0x1a4);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, NumBuyableThings) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, NumSellableThings) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, NumWantedThings) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumClothesToSell) == 0x1d4);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumWeaponsToSell) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumItemsToSell) == 0x1dc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumClothesToBuy) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumWeaponsToBuy) == 0x1e4);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumItemsToBuy) == 0x1e8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumClothesWanted) == 0x1ec);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumWeaponsWanted) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotNumItemsWanted) == 0x1f4);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, TotSpecialItemsToBuy) == 0x1f8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, SpecialItemString_PStringData) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemBuyLists) == 0x200);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, ItemSellLists) == 0x20c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, InventoryTradeOpen) == 0x218);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, InventoryTradeInitialised) == 0x219);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, PMouse_Pointer) == 0x21c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, PTop_Level_List) == 0x220);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, PCurrentOpenTradeMenu) == 0x224);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryTrade, AccumFadeTime) == 0x228);

#endif // FABLE_ENGINE_CTCINVENTORYTRADE_H
#endif // FABLE_RETAIL_CTCINVENTORYTRADE
