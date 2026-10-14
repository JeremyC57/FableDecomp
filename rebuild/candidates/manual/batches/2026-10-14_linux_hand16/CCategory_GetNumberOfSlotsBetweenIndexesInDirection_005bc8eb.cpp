#pragma optimize("s", on)
// CCategory::GetNumberOfSlotsBetweenIndexesInDirection(long, long, long) -- steps from one slot index to another in the given direction, wrapping at both ends, and counts the steps
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC8EB, length 73 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: none.
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
// The wrap tests are signed (`jl` / `jge`) against the slot count, so size() is converted to long.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
};
// Retail std::vector<CItem> (12 bytes). size() is the signed element difference (`sub ; cdq ; idiv 14h`)
// converted to the unsigned size_type; elements are reached through `first`.
struct CItemVector {
    CItem_Methods* first;                        // +0x00
    CItem_Methods* last;                         // +0x04
    CItem_Methods* end_of_storage;               // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    bool empty() const { return first == last; }
    CItem_Methods& operator[](unsigned long n) { return first[n]; }
};

struct CCategory_Retail {
    unsigned char PAssociatedThing[0x8];         // +0x00 CIntelligentPointer<CThing>
    CItemVector   ItemsInCategory;               // +0x08
    long GetNumberOfSlotsBetweenIndexesInDirection(long fromIndex, long toIndex, long direction);
};

long CCategory_Retail::GetNumberOfSlotsBetweenIndexesInDirection(long fromIndex, long toIndex, long direction)
{
    long count = 0;
    long index = fromIndex;
    while (index != toIndex) {
        index += direction;
        if (index >= (long)ItemsInCategory.size())
            index = 0;
        else if (index < 0)
            index = (long)ItemsInCategory.size() - 1;
        ++count;
    }
    return count;
}
