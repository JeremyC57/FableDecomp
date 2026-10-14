#pragma optimize("s", on)
// CCategory::GetAmountOfItemsContainable(long) -- how many more items of the type fit: the remaining room of its slot, or a whole stack when an empty slot exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF1D7, length 74 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: stack size query @ 0x005D99F0 (__fastcall, the item type in ecx; manifest "GetInventoryCategory", propagated), CCategory::GetSlotWithItemsOfType @ 0x005BE85B, CCategory::GetFirstEmptySlot @ 0x005BD96B (landed).
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
// The stack size is a named local read before the slot lookup (edi across both calls); the empty-slot
// branch is the branch-free conditional (`inc ; neg ; sbb ; and`).
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
long __fastcall GetMaxItemsPerSlot(long itemType);   // 0x005D99F0

struct CCategory_Retail {
    unsigned char PAssociatedThing[0x8];         // +0x00 CIntelligentPointer<CThing>
    CItemVector   ItemsInCategory;               // +0x08
    long GetSlotWithItemsOfType(long itemType); // 0x005BE85B
    long GetFirstEmptySlot();                   // 0x005BD96B
    long GetAmountOfItemsContainable(long itemType);
};

long CCategory_Retail::GetAmountOfItemsContainable(long itemType)
{
    long maxPerSlot = GetMaxItemsPerSlot(itemType);
    long slot = GetSlotWithItemsOfType(itemType);
    if (slot == -1)
        return GetFirstEmptySlot() != -1 ? maxPerSlot : 0;
    long count = ItemsInCategory[slot].NumberOfItems;
    if (count < maxPerSlot)
        return maxPerSlot - count;
    return 0;
}
