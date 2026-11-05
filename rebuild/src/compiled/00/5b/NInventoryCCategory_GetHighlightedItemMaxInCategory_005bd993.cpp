// NInventory::CCategory::GetHighlightedItemMaxInCategory() const -- zero when the highlighted slot is empty, otherwise that
// item's per-slot maximum (NInventory::CItem::GetMaximumNumberOfItems 0x005BD931, tail-jumped).
// Leaf: ?GetHighlightedItemMaxInCategory@CCategory@NInventory@@QBEJXZ
// Identity: Ego_r.pdb 0x00620897 + FableWin.pdb 0x023622A9 (?GetHighlightedItemMaxInCategory@CCategory@NInventory@@QBEJXZ).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD993, length 31 bytes; size-optimised TU (as the landed same-TU
//   siblings 0x005BD931 / 0x005BD96B / 0x005BD9B2: optimize("s", on)).
// Shape: the zero result is a named local taken first (`xor eax,eax` then `cmp [..],eax`), and the index is converted
//   to the vector's size_type once: VC7.1 then keeps the scaled offset (`lea` + `shl ecx,2`) apart from the vector base
//   in esi, as retail does. Plain C++ (asm cleanup round 2b; replaces the oracle byte bake).
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
#include "engine/CItem.h"
#include "engine/CCategory.h"
namespace NInventory {
class CItem : public ::CItem { public:                  // NInventory::CItem over the shared engine/CItem.h layout
    long GetMaximumNumberOfItems() const;              // 0x005BD931
};
class CCategory : public ::CCategory { public:          // NInventory::CCategory over the shared engine/CCategory.h layout
    long GetHighlightedItemMaxInCategory() const;
private:
    // ItemsInCategory (+0x08) is the retail STLport vector<NInventory::CItem> (12 bytes, opaque in the generated header).
    const std::vector<CItem>& Items() const { return *reinterpret_cast<const std::vector<CItem>*>(ItemsInCategory); }
};

long CCategory::GetHighlightedItemMaxInCategory() const
{
    long result = 0;
    size_t index = HighlightedItemIndex;
    if (Items()[index].NumberOfItems != 0)
        result = Items()[index].GetMaximumNumberOfItems();
    return result;
}
}
