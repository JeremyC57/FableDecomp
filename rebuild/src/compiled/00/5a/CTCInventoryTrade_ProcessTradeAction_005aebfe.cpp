// CTCInventoryTrade::ProcessTradeAction() -- performs the trade for the selected category when trading is allowed: in the buy category the highlighted item is applied as a tattoo card or a hair card when it is one, otherwise bought; in the sell category the highlighted item is sold
// Leaf: ?ProcessTradeAction@CTCInventoryTrade@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005AEBFE, length 194 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: NInventory::CTCInventoryBase::GetSelectedCategory() @ 0x005BCACE, NInventory::CCategory::GetCategoryIdentifier() @ 0x005BD1B3, the
//   virtual GetIsEnabled (slot 75, +0x12C), GetHighlightedItem() @ 0x005BCAE3, CTCInventoryItem::GetTattooCardDef / GetHairCardDef @ 0x005D9AA0 /
//   0x005D9A10 (static `__fastcall`, a counted def pointer by value through the hidden pointer), ApplyTattooCard(long) @ 0x005AD305,
//   ApplyHairCard(const CHairCardDef&) @ 0x0059D9C2, BuyHighlightedItem() @ 0x005A3581 and SellHighlightedItem() @ 0x005A3436.
// Shape: the category identifier starts at zero and is a two-case switch (trade buy 11 / trade sell 12: `sub eax,0Bh ; je ; dec eax ; jne`);
//   the two card defs are by-value temporaries released after their null tests, inner first.
// Name fix (agentNF89, q11170): Ego_r.pdb + FableWin.pdb ?ProcessTradeAction@CTCInventoryTrade@@UAEXXZ (CTCInventoryTrade is not in NInventory; the
//   slot-68 override). Moved off the row-local NInventory::CTCInventoryTrade / CCategory / CDefPointer / card-def models onto
//   engine/CTCInventoryTrade.h (overlay opt-in CTCINVENTORYTRADE_PROCESSTRADEACTION: the override, GetIsEnabled and SellHighlightedItem),
//   NInventory_CTCInventoryBase.h, CCategory.h (CCATEGORY_CATEGORYIDENTIFIER) and CTCInventoryItem.h (CARDDEFS).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_HIGHLIGHTEDITEM
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_CATEGORYIDENTIFIER
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYHIGHLIGHTEDITEM
#define FABLE_RETAIL_CTCINVENTORYTRADE_PROCESSTRADEACTION
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CHAIRCARDDEF
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
void CTCInventoryTrade::ProcessTradeAction()
{
    long category = 0;
    NInventory::CCategory* selected = GetSelectedCategory();
    if (selected)
        category = selected->GetCategoryIdentifier();
    if (GetIsEnabled()) {
        switch (category) {
        case NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_BUY: {
            long item = GetHighlightedItem();
            if (item > 0) {
                CDefPointer<const CTattooDef> tattoo = CTCInventoryItem::GetTattooCardDef(item);
                if (!tattoo.IsNull()) {
                    ApplyTattooCard(item);
                } else {
                    CDefPointer<const CHairCardDef> hair = CTCInventoryItem::GetHairCardDef(item);
                    if (!hair.IsNull())
                        ApplyHairCard(*hair);
                    else
                        BuyHighlightedItem();
                }
            }
            break;
        }
        case NInventory::INVENTORY_CATEGORY_IDENTIFIER_TRADE_SELL:
            SellHighlightedItem();
            break;
        }
    }
}
