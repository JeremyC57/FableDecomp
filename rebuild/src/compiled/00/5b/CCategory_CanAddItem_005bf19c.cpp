#pragma optimize("s", on)
// CCategory::CanAddItem(long) -- an item type can be added when its existing slot is not full, or else when there is an empty slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF19C, length 59 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: CCategory::GetSlotWithItemsOfType @ 0x005BE85B, CItem::IsSlotFull @ 0x005BDE50 (bool), CCategory::GetFirstEmptySlot @ 0x005BD96B (landed).
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
// The first return is the branch-free `!IsSlotFull()` (`neg al ; sbb ; inc`), the second the full-register bool (`setne cl ; mov al,cl`).
#include "engine/CItem.h"

struct CItem_Methods : CItem {
    bool IsSlotFull();                        // 0x005BDE50
};
// Retail std::vector<CItem> (12 bytes). size() is the signed element difference (`sub ; cdq ; idiv 14h`)
// converted to the unsigned size_type; elements are reached through `first`.
struct CItemVector {
    CItem_Methods* first;                        // +0x00
    CItem_Methods* last;                         // +0x04
    CItem_Methods* end_of_storage;               // +0x08
    struct iterator {                            // iterator OBJECT: `*(begin() + n)` is not folded like `first[n]`
        CItem_Methods* p;
        explicit iterator(CItem_Methods* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        CItem_Methods& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    CItem_Methods& operator[](long n) { return *(begin() + n); }
};

struct CCategory_Retail {
    unsigned char PAssociatedThing[0x8];         // +0x00 CIntelligentPointer<CThing>
    CItemVector   ItemsInCategory;               // +0x08
    long GetSlotWithItemsOfType(long itemType); // 0x005BE85B
    long GetFirstEmptySlot();                   // 0x005BD96B
    bool CanAddItem(long itemType);
};

bool CCategory_Retail::CanAddItem(long itemType)
{
    long slot = GetSlotWithItemsOfType(itemType);
    if (slot != -1) {
        CItem_Methods* item = &ItemsInCategory[slot];
        return !item->IsSlotFull();
    }
    return GetFirstEmptySlot() != -1;
}