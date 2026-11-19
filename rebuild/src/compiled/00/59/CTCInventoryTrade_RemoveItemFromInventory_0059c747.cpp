// CTCInventoryTrade::RemoveItemFromInventory(long, long) -- removes `count` items of a type from the category of the current trade type, if there is one
// Leaf: ?RemoveItemFromInventory@CTCInventoryTrade@@UAEXJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C747, 29 bytes. rtti-vtables: CTCInventoryTrade 0x01253034 slot 79.
// Callees: CTCInventoryTrade::GetCategoryIndexForTradeType @ 0x0059B6F4, NInventory::CTCInventoryBase::GetCategoryWithIndex(long)
// @ 0x005BCE3C, NInventory::CCategory::RemoveItem(long, long) @ 0x005BF221 (tail jump reusing the two argument slots).
// Shape: `mov edx,ecx ; call GetCategoryIndexForTradeType ; push eax ; mov ecx,edx` -- `this` survives the first call in EDX, which only
// works because VC7.1 knows the registers a function compiled EARLIER IN THE SAME TRANSLATION UNIT clobbers (0x0059B6F4 touches only
// eax/ecx); see the same-TU body below (the landed 0x0059C6B0 / 0x0059C6DD / 0x0059C710 form). Size TU.
// Name fix (agentNF94, q11226): Ego_r.pdb + FableWin.pdb ?RemoveItemFromInventory@CTCInventoryTrade@@UAEXJJ@Z (public virtual override of
//   NInventory::CTCInventoryBase::RemoveItemFromInventory(long, long), CTCInventoryTrade vtable 0x01253034 slot 79 = 0x0059C747; engine/
//   CTCInventoryTrade.h declares it under FABLE_RETAIL_CTCINVENTORYBASE_VTABLE). The old row defined ?vfunc_79@CTCInventoryTrade_Retail@@ on a
//   local CTCInventoryTrade_Retail / CTCInventoryBase_Retail / CCategory model and called a _Retail copy of GetCategoryIndexForTradeType; it
//   now sits on the shared headers and binds the PDB spellings.
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
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER
#define FABLE_RETAIL_CCATEGORY_REMOVEITEM
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_TRADECATEGORY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
// Same-TU body of GetCategoryIndexForTradeType 0x0059B6F4 (kept out of line), the landed 0x0059C6B0 text unchanged: VC7.1 then knows it
//   touches only eax/ecx, so `this` survives the call in edx as retail does (declared only, the call clobbers edx and VC uses `push esi`).
//   The table keeps the landed descriptive spelling (no PDB or label names the .data pair at 0x0137CCDC).
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
void CTCInventoryTrade::RemoveItemFromInventory(long itemDefIndex, long count)
{
    NInventory::CCategory* pcategory = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    if (pcategory)
        pcategory->RemoveItem(itemDefIndex, count);
}
