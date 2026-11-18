// CTCInventoryTrade::RemoveAllItemsFromCategory(NInventory::EInventoryCategoryIdentifier category) -- remembers the category and removes every (item type, count) entry of the trade category from the inventory
// Leaf: ?RemoveAllItemsFromCategory@CTCInventoryTrade@@QAEXW4EInventoryCategoryIdentifier@NInventory@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059CF49, length 116 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryTrade::GetCategoryIndexForTradeType() @ 0x0059B6F4, NInventory::CTCInventoryBase::GetCategoryWithIndex(long) @ 0x005BCE3C,
//   NInventory::CCategory::GetAllItemsInCategory(vector<pair<long,long> >&) const @ 0x005BF2C6, virtual slot 79 (+0x13C,
//   RemoveItemFromInventory(long, long)), the vector's storage freed by 0x00BFEA14.
// Shape: the category is a named local looked up before the vector is declared; `(long)size()` gives the signed `sar 3 ; jle` count.
// Name fix (agentNF89, q11166): Ego_r.pdb + FableWin.pdb ?RemoveAllItemsFromCategory@CTCInventoryTrade@@QAEXW4EInventoryCategoryIdentifier@NInventory@@@Z
//   (the argument is the namespace enum, not long). Moved off the row-local CTCInventoryTrade / CCategory / item-vector models onto the shared
//   headers (VTABLE for slot 79, PASS22 CCategory, STLport vector<pair<long,long> >; overlay CTCINVENTORYTRADE_TRADECATEGORY).
#pragma optimize("y", off)
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
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
void CTCInventoryTrade::RemoveAllItemsFromCategory(NInventory::EInventoryCategoryIdentifier category)
{
    CategoryIdent = (EInventoryCategoryIdentifier)category;
    NInventory::CCategory* tradeCategory = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    std::vector<std::pair<long, long> > items;
    tradeCategory->GetAllItemsInCategory(items);
    long count = (long)items.size();
    for (long i = 0; i < count; ++i)
        RemoveItemFromInventory(items[i].first, items[i].second);
}
