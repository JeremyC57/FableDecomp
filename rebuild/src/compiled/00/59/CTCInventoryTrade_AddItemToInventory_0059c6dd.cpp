// CTCInventoryTrade::AddItemToInventory(CThing& item, bool add_selected, bool add_quick_access, long price_bought_for, bool silent) -- vtable
//   slot 80: add the thing to the category of the current trade type (selected flag and price passed on); true on success
// Leaf: ?AddItemToInventory@CTCInventoryTrade@@UAE_NAAVCThing@@_N1J1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C6DD, 51 bytes. rtti-vtables: CTCInventoryTrade 0x01253034 slot 80 (`ret 14h`).
// Callees: CTCInventoryTrade::GetCategoryIndexForTradeType 0x0059B6F4, NInventory::CTCInventoryBase::GetCategoryWithIndex(long) 0x005BCE3C,
//   NInventory::CCategory::AddItem(CThing&, bool, long) 0x005BF5A4 (called with arguments 1, 2, 4; add_quick_access and silent unused; was bound
//   as the pointer form ?AddItem@CCategory@NInventory@@QAE_NPAVCThing@@_NJ@Z).
// Shape: `mov edx,ecx ; call GetCategoryIndexForTradeType ; push eax ; mov ecx,edx` -- `this` survives the first call in EDX, which only
//   works because VC7.1 knows the registers a function compiled EARLIER IN THE SAME TRANSLATION UNIT clobbers (0x0059B6F4 touches only
//   eax/ecx): the same-TU body below.
//   `if (pcategory && AddItem(...)) return true; return false;` gives the retail `test al,al ; je ; mov al,1 ; jmp` exits.
// Identity (NF92, q11215): Ego_r.pdb 0x00608E2B + FableWin.pdb 0x02314422 ?AddItemToInventory@CTCInventoryTrade@@UAE_NAAVCThing@@_N1J1@Z (public
//   virtual, the 5-argument overload: `ret 14h`); Ego_d.pdb names the parameters and the local pcategory. Was the made-up vfunc_80 on a local
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
#define FABLE_RETAIL_CCATEGORY_ADDITEM_THING
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
// Same-TU body of GetCategoryIndexForTradeType 0x0059B6F4 (landed; kept out of line), as in the landed 0x0059C6B0 (NF89) text unchanged: VC7.1
//   then knows it touches only eax/ecx, so `this` survives the call in edx as retail does. The shared-header inline body (NF91 opt-in
//   TRADECATEGORY_BODY) gives DIFFER 47v45 on 0x0059C6B0 and 57v55 here (NF91 / NF92); this strong copy joins the open one-definition entry
//   q11204 with 0x0059C6B0. The table keeps the landed 0x0059B6F4 row's descriptive spelling.
struct CTradeTypeCategory {
    long Ident;          // EInventoryCategoryIdentifier value
    long CategoryIndex;  // trade category index
};
extern CTradeTypeCategory g_TradeTypeCategories[2];    // 0x0137CCDC: {11,0}, {12,1}
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
bool CTCInventoryTrade::AddItemToInventory(CThing& item, bool add_selected, bool add_quick_access, long price_bought_for, bool silent)
{
    NInventory::CCategory* pcategory = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    if (pcategory && pcategory->AddItem(item, add_selected, price_bought_for))
        return true;
    return false;
}
