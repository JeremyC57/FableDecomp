// NInventory::CCategory::GetUniqueItemCountInCategory() const -- counts the non-empty item stacks of the category
// Leaf: ?GetUniqueItemCountInCategory@CCategory@NInventory@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC8AC, length 63 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: none.
// Name fix (agentNF89, q11168): Ego_r.pdb + FableWin.pdb ?GetUniqueItemCountInCategory@CCategory@NInventory@@QBEJXZ (const); the landed
//   0x0059D820 binds it. Moved off the row-local CCategory_Retail / CItem_Methods / CItemVector models onto engine/CCategory.h (PASS22 class,
//   STLport vector<NInventory::CItem> of 20-byte items; overlay opt-in FABLE_RETAIL_CCATEGORY_UNIQUEITEMCOUNT) and NInventory::CItem::IsSlotEmpty.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_UNIQUEITEMCOUNT
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
long CCategory::GetUniqueItemCountInCategory() const
{
    long count = 0;
    for (unsigned long i = 0; i < ItemsInCategory.size(); ++i) {
        if (ItemsInCategory[i].NumberOfItems != 0)
            ++count;
    }
    return count;
}
}
