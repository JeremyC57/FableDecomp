// CCategory::GetPreviousOccupiedSlot(long index) -- the last occupied inventory slot before `index`; when the category wraps its highlight cursor, continue from the last slot down to `index`; `index` itself when nothing is found
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCFE2, length 114 bytes (ret 4); size-optimised TU (`push 14h ; pop
// ecx` divisor). Called by CCategory::HighlightPreviousItem (0x005BD0F7).
// Callees: CInventoryCategoryDef virtual slot 1 (+0x04, destroy) when the def's count drops to zero.
// Shape: both scans are SIGNED downward loops with the slot pointer strength-reduced (`sub ecx,14h`); the wrapped scan
// starts at `(long)size() - 1`; WrapHighlightCursor (+0x46) is read through the scoped CDefPointer reference.
// Leaf: ?GetPreviousOccupiedSlot@CCategory@NInventory@@ABEJJ@Z
// Name fix (NF96 q11446): Ego_r.pdb 0x0061FF10 + FableWin.pdb 0x02361C5F ?GetPreviousOccupiedSlot@CCategory@NInventory@@ABEJJ@Z (private const),
//   the twin of GetNextOccupiedSlot 0x005BCF61 (engine/CCategory.h ABE). Shared headers: engine/CCategory.h (overlay opt-in
//   FABLE_RETAIL_CCATEGORY_GETPREVIOUSOCCUPIEDSLOT) with the define set of the landed twin HighlightNextItem 0x005BD054 (GetPDef: the counted
//   def copy the old local CDefRef modelled); was ?GetPreviousOccupiedSlot@CCategory_Retail@@QAEJJ@Z on a local CCategory_Retail view.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_UNIQUEITEMCOUNT
#define FABLE_RETAIL_CCATEGORY_GETPREVIOUSOCCUPIEDSLOT
#define FABLE_RETAIL_CCATEGORY_GETPDEF
#define FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/_quarantine/CInventoryCategoryDef.h"
namespace NInventory {
long CCategory::GetPreviousOccupiedSlot(long index) const
{
    for (long i = index - 1; i >= 0; --i) {
        if (ItemsInCategory[i].NumberOfItems != 0)
            return i;
    }
    if (GetPDef()->WrapHighlightCursor) {
        for (long j = (long)ItemsInCategory.size() - 1; j >= index; --j) {
            if (ItemsInCategory[j].NumberOfItems != 0)
                return j;
        }
    }
    return index;
}
}
