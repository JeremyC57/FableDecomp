// NInventory::CCategory::GetAmountOfItemsContainable(long itemDefIndex) const -- how many more items of the type fit: the remaining room of its
//   stack, or a whole stack when an empty slot exists
// Leaf: ?GetAmountOfItemsContainable@CCategory@NInventory@@QBEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF1D7, 74 bytes, `ret 4`; size-optimised TU.
// Name: Ego_r.pdb 0x00621FB3 ?GetAmountOfItemsContainable@CCategory@NInventory@@QBEJJ@Z (after IsSpaceInCategoryForItem 0x00621F79 = retail
//   0x005BF19C); the tail-jump target of NInventory::CTCInventoryBase::GetAmountOfItemsContainable 0x005BF30D (q11154 / q11158).
// Callees: CTCInventoryItem::GetMaxNumberOfItems 0x005D99F0 (static __fastcall), CCategory::GetSlotWithItemsOfType 0x005BE85B (Ego_r.pdb
//   ?GetSlotWithItemsOfType@CCategory@NInventory@@ABEJJ@Z, private const), CCategory::GetFirstEmptySlot 0x005BD96B (?GetFirstEmptySlot@..@@ABEJXZ).
// Shape: the stack size is a named local read before the slot lookup (edi across both calls); the empty-slot branch is the branch-free
//   conditional (`inc ; neg ; sbb ; and`).
// Port (NF88, round 7, q11158): the local CCategory_Retail / CItem_Methods / CItemVector models are gone: the row is on
//   engine/NInventory_CTCInventoryBase.h (NInventory::CItem, CONFISCATED) + engine/CCategory.h (the PASS22 NInventory::CCategory, NF88 opt-in
//   FABLE_RETAIL_CCATEGORY_ITEMQUERIES) + engine/CTCInventoryItem.h (GETMAXNUMBEROFITEMS, NF64's binding kept).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_GETMAXNUMBEROFITEMS
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryItem.h"

namespace NInventory {
long CCategory::GetAmountOfItemsContainable(long itemDefIndex) const
{
    long maxPerSlot = CTCInventoryItem::GetMaxNumberOfItems(itemDefIndex);
    long slot = GetSlotWithItemsOfType(itemDefIndex);
    if (slot == -1)
        return GetFirstEmptySlot() != -1 ? maxPerSlot : 0;
    long count = ItemsInCategory[slot].NumberOfItems;
    if (count < maxPerSlot)
        return maxPerSlot - count;
    return 0;
}
}
