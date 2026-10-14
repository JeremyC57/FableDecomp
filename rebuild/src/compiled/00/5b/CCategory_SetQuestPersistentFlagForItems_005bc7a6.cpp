#pragma optimize("s", on)
// CCategory::SetQuestPersistentFlagForItems(bool) -- sets the quest-persistent flag on every non-empty item stack of the category
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC7A6, length 72 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: CItem::SetQuestPersistentFlag(bool) @ 0x005BC6F9 (landed).
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
    void SetQuestPersistentFlag(bool flag);   // 0x005BC6F9
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
    void SetQuestPersistentFlagForItems(bool flag);
};

void CCategory_Retail::SetQuestPersistentFlagForItems(bool flag)
{
    for (unsigned long i = 0; i < ItemsInCategory.size(); ++i) {
        if (ItemsInCategory[i].NumberOfItems != 0)
            ItemsInCategory[i].SetQuestPersistentFlag(flag);
    }
}