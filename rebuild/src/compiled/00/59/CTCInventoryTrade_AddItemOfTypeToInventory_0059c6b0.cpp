// CTCInventoryTrade::AddItemOfTypeToInventory(long, long) -- adds `count` items of a type to the category of the current trade type (not silently, price -1); false when there is no such category
// Leaf: ?AddItemOfTypeToInventory@CTCInventoryTrade@@AAE_NJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C6B0, 45 bytes. Callees: CTCInventoryTrade::GetCategoryIndexForTradeType @ 0x0059B6F4,
//   NInventory::CTCInventoryBase::GetCategoryWithIndex(long) @ 0x005BCE3C, NInventory::CCategory::AddItem(long, bool, long, long) @ 0x005BF060
//   (bool in al, returned as is).
// Shape: `mov edx,ecx ; call GetCategoryIndexForTradeType ; push eax ; mov ecx,edx` (see the same-TU body below); size TU.
// Name fix (agentNF89, q11166): Ego_r.pdb + FableWin.pdb ?AddItemOfTypeToInventory@CTCInventoryTrade@@AAE_NJJ@Z (private; engine/CTCInventoryTrade.h
//   ADDITEMOFTYPE declares it, the landed 0x0059D820 binds it). Moved off the row-local CTCInventoryTrade / CTCInventoryBase_Retail / CCategory
//   models onto the shared headers: CategoryIdent +0x170, GetCategoryWithIndex (CATEGORYWITHINDEX), CCategory::AddItem (overlay CCATEGORY_ADDITEM),
//   GetCategoryIndexForTradeType (overlay CTCINVENTORYTRADE_TRADECATEGORY).
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
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCATEGORY_ADDITEM
#define FABLE_RETAIL_CTCINVENTORYTRADE_ADDITEMOFTYPE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
// Same-TU body of GetCategoryIndexForTradeType 0x0059B6F4 (landed; kept out of line): VC7.1 then knows it touches only eax/ecx, so `this`
//   survives the call in edx as retail does (declared only, the call clobbers edx). The table keeps the landed 0x0059B6F4 row's descriptive
//   spelling (no PDB or label names the .data pair at 0x0137CCDC).
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
bool CTCInventoryTrade::AddItemOfTypeToInventory(long itemType, long count)
{
    NInventory::CCategory* category = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    if (category)
        return category->AddItem(itemType, false, -1, count);
    return false;
}
