// NInventory::CCategory::AddInventoryItem(const CItem& item) -- merge an inventory item into the category: onto its stack or into the first empty
//   slot (false when there is none or the slot is full), highlighting that slot when the highlighted one is empty
// Leaf: ?AddInventoryItem@CCategory@NInventory@@QAE_NABVCItem@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF103, 107 bytes, `ret 4`; size-optimised TU.
// Name (NF90): Ego_r.pdb 0x00621EE3 + FableWin.pdb ?AddInventoryItem@CCategory@NInventory@@QAE_NABVCItem@2@@Z, second of the TU run AddItem /
//   AddInventoryItem / UpdateItemStatsForThing / IsSpaceInCategoryForItem = retail 0x005BF060 / 0x005BF103 / 0x005BF16E / 0x005BF19C.
// Callees: GetSlotWithItemsOfType 0x005BE85B / GetFirstEmptySlot 0x005BD96B (Ego_r private const), CItem::IsSlotFull 0x005BDE50, CItem::AddInventoryItem
//   0x005BDDE4 (Ego_r.pdb / FableWin.pdb ?AddInventoryItem@CItem@NInventory@@QAEXABV12@@Z).
// Port (NF90, q11182 / q11183 binder, claimed through the lock): was a local CCategory_Retail model (leaf
//   ?AddInventoryItem@CCategory_Retail@@QAE_NABUCItem@@@Z) binding both entry addresses under CCategory_Retail spellings; now on
//   engine/NInventory_CTCInventoryBase.h (CONFISCATED CItem, NF90 opt-ins _CITEM_ISSLOTFULL / _CITEM_ADDINVENTORYITEM) + engine/CCategory.h (PASS22,
//   NF88 _ITEMQUERIES, NF90 opt-in _ADDINVENTORYITEM).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CCATEGORY_ADDINVENTORYITEM
#define FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL
#define FABLE_RETAIL_NINVENTORY_CITEM_ADDINVENTORYITEM
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
bool CCategory::AddInventoryItem(const CItem& item)
{
    long slot = GetSlotWithItemsOfType(item.ItemType);
    if (slot == -1) {
        slot = GetFirstEmptySlot();
        if (slot == -1)
            return false;
    }
    if (ItemsInCategory[slot].IsSlotFull())
        return false;
    ItemsInCategory[slot].AddInventoryItem(item);
    if (ItemsInCategory[HighlightedItemIndex].IsSlotEmpty())
        HighlightedItemIndex = slot;
    return true;
}
}
