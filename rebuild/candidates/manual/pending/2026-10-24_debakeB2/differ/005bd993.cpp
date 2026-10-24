// Grade: DIFFER(25v31) -- unchanged best (debakeB). debakeB2 tried 6 more: char* byte-offset cast (26), ternary (25), idx[first] / idx+first (25), /O1 (25), unsigned byte-offset local (27), #pragma optimize("g", off) (62v31 -- frame and memory locals, but it is the only form that gives retail's `cmp [ecx+eax+4]` with the base added only on the tail-jmp path, i.e. retail did not CSE base+offset).
// NInventory::CCategory::GetMaxItemsPerSlot() -- maximum item count of the highlighted slot (0 for an empty slot)
// Retail: Fable.exe @ 0x005BD993, 31 bytes, size-optimised TU (unaligned entry, the CCategory/CItem code around it).
// Indexes the 20-byte CItem vector by HighlightedItemIndex (`lea [edx+edx*4]` / `shl 2`), returns 0 when the slot
// holds no items and otherwise tail-jumps into CItem::GetMaximumNumberOfItems (0x005BD931, landed) on the slot.
// Caller: 0x005BE2B1. Manifest label CRenderTarget::DetachTarget was propagated.
// Retail CCategory: the PDB header (engine/CCategory.h) has a 16-byte vector; the shipped STLport vector is 12 bytes,
// so HighlightedItemIndex is +0x24, not +0x28 (as the landed 0x005BD9B2 / 0x005BD1CF document). Local retail view.
#pragma optimize("s", on)
#include "engine/CItem.h"
struct CItem_Methods : CItem {
    long GetMaximumNumberOfItems();           // 0x005BD931
};
struct CItemVector {                          // retail std::vector<NInventory::CItem>, 12 bytes
    CItem_Methods* first;                     // +0x00
    CItem_Methods* last;                      // +0x04
    CItem_Methods* end_of_storage;            // +0x08
    CItem_Methods& operator[](unsigned long n) { return first[n]; }
};
struct CCategory_Retail {
    unsigned char     PAssociatedThing[0x8];  // +0x00 CIntelligentPointer<CThing>
    CItemVector       ItemsInCategory;        // +0x08
    void*             PDef_Object;            // +0x14 CDefPointer<const CInventoryCategoryDef>
    void*             PInventory;             // +0x18 CTCInventoryBase*
    long              GlobalDefIndex;         // +0x1c
    long              SelectedItemIndex;      // +0x20
    long              HighlightedItemIndex;   // +0x24
    bool              Selected;               // +0x28
    bool              DisplayItems;           // +0x29
    long GetMaxItemsPerSlot();
};
long CCategory_Retail::GetMaxItemsPerSlot()
{
    long maximum = 0;
    if (ItemsInCategory[HighlightedItemIndex].NumberOfItems != 0)
        maximum = ItemsInCategory[HighlightedItemIndex].GetMaximumNumberOfItems();
    return maximum;
}
