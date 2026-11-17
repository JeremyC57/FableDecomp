// CTCInventoryTrade::GetItemCountForCategory() -- the buy or sell item count matching the selected category (trade buy / trade sell), else 0
// Leaf: ?GetItemCountForCategory@CTCInventoryTrade@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B740, 55 bytes.
// Callees: NInventory::CTCInventoryBase::GetSelectedCategory @ 0x005BCACE, NInventory::CCategory::GetCategoryIdentifier @ 0x005BD1B3.
// Shape: both locals zeroed up front (`xor edi,edi ; xor ebx,ebx`), the identifier only fetched for a non-null category, then a two-case
//   `switch` lowered to `sub ebx,0Bh ; je ; dec ebx ; jne` (GOTCHAS: `dec ; jne` = a switch case). Size TU.
// Name fix (agentNF89, q11170): Ego_r.pdb + FableWin.pdb ?GetItemCountForCategory@CTCInventoryTrade@@QAEJXZ; moved off the row-local
//   CTCInventoryTrade_Retail / CTCInventoryBase_Retail / CCategory / empty-enum models onto engine/CTCInventoryTrade.h (ItemSellCount +0x164 /
//   ItemBuyCount +0x168), NInventory_CTCInventoryBase.h (SELECTEDOWNED) and CCategory.h (PASS22), with the PDB enumerators
//   INVENTORY_CATEGORY_IDENTIFIER_TRADE_BUY (11) / _TRADE_SELL (12). Overlay opt-ins CTCINVENTORYTRADE_ITEMCOUNTFORCATEGORY,
//   CCATEGORY_CATEGORYIDENTIFIER.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_ITEMCOUNTFORCATEGORY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
long CTCInventoryTrade::GetItemCountForCategory()
{
    long count = 0;
    long ident = 0;
    NInventory::CCategory* category = GetSelectedCategory();
    if (category)
        ident = category->GetCategoryIdentifier();
    switch (ident) {
    case NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_BUY:
        count = ItemBuyCount;
        break;
    case NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_SELL:
        count = ItemSellCount;
        break;
    }
    return count;
}
