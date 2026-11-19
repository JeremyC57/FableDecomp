// CCategory::HighlightPreviousItem() -- move the inventory highlight one slot back: categories that select empty slots step to the previous slot (wrapping to the last slot or clamping at 0), others jump to the previous occupied slot; tell the inventory when the highlight changed
// Shape: the mirror of the landed HighlightNextItem 0x005BD054: `if (h - 1 >= 0) { --h; if (h < 0) { wrap/clamp } }` --
// retail tests the decremented copy (`dec eax ; js`), stores it and re-tests the member (`test eax,eax ; jge`), so the
// wrap/clamp block is unreachable (an index of 0 stays 0); VC7.1 does not fold the second test into the first.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD0F7, length 140 bytes; size-optimised TU (`push 14h ; pop ecx`,
// `and dword ptr [esi+24h],0`, pass-through `push dword ptr [esi+24h]`).
// Callees: CCategory::GetPreviousOccupiedSlot @ 0x005BCFE2, CInventoryCategoryDef virtual slot 1 (+0x04, destroy),
// CTCInventoryBase virtual slot 36 (+0x90, the highlight-changed notification with the item type).
// Leaf: ?HighlightPreviousItem@CCategory@NInventory@@QAEXXZ
// Name fix (NF96 q11445 / q11446): Ego_r.pdb 0x00620025 + FableWin.pdb 0x02362ABB ?HighlightPreviousItem@CCategory@NInventory@@QAEXXZ
//   (engine/CCategory.h FABLE_RETAIL_CCATEGORY_HIGHLIGHTPREVIOUSITEM, NF95; the ported binder 0x005BDCDC binds it so), the mirror of the
//   landed HighlightNextItem 0x005BD054, whose define set and spelling it takes; GetPreviousOccupiedSlot 0x005BCFE2 now binds its PDB
//   spelling ?GetPreviousOccupiedSlot@CCategory@NInventory@@ABEJJ@Z (overlay opt-in FABLE_RETAIL_CCATEGORY_GETPREVIOUSOCCUPIEDSLOT). Was
//   ?HighlightPreviousItem@CCategory_Retail@@QAEXXZ on local CCategory_Retail / def / vector / CTCInventoryBase models.
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
#define FABLE_RETAIL_CCATEGORY_HIGHLIGHTPREVIOUSITEM
#define FABLE_RETAIL_CCATEGORY_GETPREVIOUSOCCUPIEDSLOT
#define FABLE_RETAIL_CCATEGORY_GETPDEF
#define FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/_quarantine/CInventoryCategoryDef.h"
namespace NInventory {
void CCategory::HighlightPreviousItem()
{
    long previous = HighlightedItemIndex;
    if (GetPDef()->SelectEmptySlots) {
        if (HighlightedItemIndex - 1 >= 0) {
            --HighlightedItemIndex;
            if (HighlightedItemIndex < 0) {
                if (GetPDef()->WrapHighlightCursor)
                    HighlightedItemIndex = ItemsInCategory.size() - 1;
                else
                    HighlightedItemIndex = 0;
            }
        }
    } else {
        HighlightedItemIndex = GetPreviousOccupiedSlot(HighlightedItemIndex);
    }
    if (HighlightedItemIndex != previous)
        PInventory->OnItemHighlighted(ItemsInCategory[HighlightedItemIndex].ItemType);
}
}
