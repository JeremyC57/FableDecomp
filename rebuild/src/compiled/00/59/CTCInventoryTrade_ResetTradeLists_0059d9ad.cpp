// CTCInventoryTrade::ResetTradeLists() -- empties both trade categories: every entry of the sell category (12), then of the buy category (11)
// Leaf: ?ResetTradeLists@CTCInventoryTrade@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D9AD, 21 bytes: `push 0Ch ; call RemoveAllItemsFromCategory ; push 0Bh ; call` with `this` kept in esi.
// Name fix (agentNF89, q11166 follow-through): Ego_r.pdb + FableWin.pdb ?ResetTradeLists@CTCInventoryTrade@@QAEXXZ (engine/CTCInventoryTrade.h declares it
//   at this address; the landed BuyHighlightedItem 0x005A3581 / SellHighlightedItem 0x005A3436 bind it). Its callee 0x0059CF49 is
//   ?RemoveAllItemsFromCategory@CTCInventoryTrade@@QAEXW4EInventoryCategoryIdentifier@NInventory@@@Z with the PDB enumerators
//   INVENTORY_CATEGORY_IDENTIFIER_TRADE_SELL (12) / _TRADE_BUY (11). Was the CMemoryAllocator::ClearPerformanceStats / Clear(int) stand-in
//   (catalog CTCInventoryTrade::ResetTradeList).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_TRADECATEGORY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
void CTCInventoryTrade::ResetTradeLists()
{
    RemoveAllItemsFromCategory(NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_SELL);
    RemoveAllItemsFromCategory(NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_BUY);
}
