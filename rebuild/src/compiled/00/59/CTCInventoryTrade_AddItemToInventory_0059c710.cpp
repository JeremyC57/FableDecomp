// CTCInventoryTrade::AddItemToInventory(long itemDefIndex, long count, bool add_selected, bool add_quick_access, long price_bought_for, bool
//   silent) -- vtable slot 81: add `count` items of a type to the category of the current trade type (selected flag and price passed on);
//   true on success
// Leaf: ?AddItemToInventory@CTCInventoryTrade@@UAE_NJJ_N0J0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C710, 55 bytes. rtti-vtables: CTCInventoryTrade 0x01253034 slot 81 (`ret 18h`).
// Callees: CTCInventoryTrade::GetCategoryIndexForTradeType 0x0059B6F4, NInventory::CTCInventoryBase::GetCategoryWithIndex(long) 0x005BCE3C,
//   NInventory::CCategory::AddItem(long, bool, long, long) 0x005BF060 (called with arguments 1, 3, 5, 2; add_quick_access and silent unused).
// Shape: `mov edx,ecx ; call GetCategoryIndexForTradeType ; push eax ; mov ecx,edx` -- `this` survives the first call in EDX, which only
//   works because VC7.1 knows the registers a function compiled EARLIER IN THE SAME TRANSLATION UNIT clobbers (0x0059B6F4 touches only
//   eax/ecx): the same-TU body below.
//   `if (pcategory && AddItem(...)) return true; return false;` gives the retail `test al,al ; je ; mov al,1 ; jmp` exits.
// Identity (NF92, q11211): Ego_r.pdb 0x00608E5E + FableWin.pdb 0x02314571 ?AddItemToInventory@CTCInventoryTrade@@UAE_NJJ_N0J0@Z (public
//   virtual, the 6-argument overload: `ret 18h`); Ego_d.pdb names the parameters and the local pcategory. Was the made-up vfunc_81 on a local
//   CTCInventoryTrade / CTCInventoryBase_Retail / NInventory::CCategory model; now on engine/CTCInventoryTrade.h (NF92 opt-in ADDITEMTOINVENTORY).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_CATEGORYWITHINDEX
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_TRADECATEGORY
#define FABLE_RETAIL_CTCINVENTORYTRADE_ADDITEMTOINVENTORY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCATEGORY_ADDITEM
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
// Same-TU body of GetCategoryIndexForTradeType 0x0059B6F4 (landed; kept out of line), as in the landed 0x0059C6B0 (NF89) text unchanged: VC7.1
//   then knows it touches only eax/ecx, so `this` survives the call in edx as retail does. The shared-header inline body (NF91 opt-in
//   TRADECATEGORY_BODY) gives DIFFER 47v45 on 0x0059C6B0 and 57v55 here (NF91 / NF92); this strong copy is accepted as a
//   retail TU-mate (advisor decision, landing nf96a; q11204 closed by NF96 part 2). The table keeps the landed 0x0059B6F4 row's descriptive spelling.
struct CTradeTypeCategory {
    long Ident;          // EInventoryCategoryIdentifier value
    long CategoryIndex;  // trade category index
};
extern CTradeTypeCategory g_TradeTypeCategories[2];    // 0x0137CCDC: {11,0}, {12,1}
// Fidelity: same-TU strong definition of 0x0059B6F4 (retail TU-mate)
#pragma auto_inline(off)
long CTCInventoryTrade::GetCategoryIndexForTradeType()
{
    long ident = CategoryIdent;
    for (long i = 0; i < 2; ++i) {
        if (g_TradeTypeCategories[i].Ident == ident)
            return g_TradeTypeCategories[i].CategoryIndex;
    }
    return -1;
}
#pragma auto_inline(on)
bool CTCInventoryTrade::AddItemToInventory(long itemDefIndex, long count, bool add_selected, bool add_quick_access, long price_bought_for, bool silent)
{
    NInventory::CCategory* pcategory = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    if (pcategory && pcategory->AddItem(itemDefIndex, add_selected, price_bought_for, count))
        return true;
    return false;
}
