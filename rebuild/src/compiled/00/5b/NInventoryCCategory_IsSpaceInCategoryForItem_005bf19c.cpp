// NInventory::CCategory::IsSpaceInCategoryForItem(long itemDefIndex) const -- whether one more item of the type fits: its stack is not full,
//   or (no stack yet) an empty slot exists
// Leaf: ?IsSpaceInCategoryForItem@CCategory@NInventory@@QBE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF19C, 59 bytes, `ret 4`; size-optimised TU.
// Name (NF90, q11184): Ego_r.pdb 0x00621F79 ?IsSpaceInCategoryForItem@CCategory@NInventory@@QBE_NJ@Z, between UpdateItemStatsForThing
//   0x00621F4C (= retail 0x005BF16E) and GetAmountOfItemsContainable 0x00621FB3 (= retail 0x005BF1D7, NF88); the landed
//   NInventory::CTCInventoryBase::IsSpaceInInventoryForItem 0x005BF2ED tail-jumps here and binds exactly this spelling.
// Callees: CCategory::GetSlotWithItemsOfType 0x005BE85B and GetFirstEmptySlot 0x005BD96B (Ego_r.pdb private const), CItem::IsSlotFull
//   0x005BDE50 (Ego_r.pdb / FableWin.pdb ?IsSlotFull@CItem@NInventory@@QBE_NXZ).
// Port (NF90): was the local CCategory_Retail / CItem_Methods / CItemVector model under the made-up catalog name CCategory::CanAddItem (leaf
//   ?CanAddItem@CCategory_Retail@@QAE_NJ@Z); now on engine/NInventory_CTCInventoryBase.h (CONFISCATED CItem + the NF90 opt-in
//   FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL) + engine/CCategory.h (PASS22 class, NF88 opt-in FABLE_RETAIL_CCATEGORY_ITEMQUERIES), the
//   0x005BF1D7 define set.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
bool CCategory::IsSpaceInCategoryForItem(long itemDefIndex) const
{
    long slot = GetSlotWithItemsOfType(itemDefIndex);
    if (slot != -1)
        return !ItemsInCategory[slot].IsSlotFull();
    return GetFirstEmptySlot() != -1;
}
}
