#pragma optimize("s", on)
// CCategory::GetAllNonEmptyInventoryItemsInCategory(std::vector<CItem>&, bool) -- copies every non-empty stack into the caller's vector; unless the flag says to take all of them only quest-persistent stacks are copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF97D, length 56 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: CItem::IsQuestPersistent @ 0x005BC729 (landed), vector<CItem>::push_back @ 0x005C2996 (__fastcall on the output vector, const reference to the item).
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
    bool IsQuestPersistent();                 // 0x005BC729
};

struct CItemVectorCalls {
    void PushBack(const CItem& item);            // 0x005C2996
};

struct CItemVector {                             // retail std::vector<CItem>
    CItem_Methods* first;                        // +0x00
    CItem_Methods* last;                         // +0x04
    CItem_Methods* end_of_storage;               // +0x08
};

struct CCategory_Retail {
    unsigned char PAssociatedThing[0x8];         // +0x00 CIntelligentPointer<CThing>
    CItemVector   ItemsInCategory;               // +0x08
    void GetAllNonEmptyInventoryItemsInCategory(CItemVectorCalls* outItems, bool includeNonQuestPersistent);
};

void CCategory_Retail::GetAllNonEmptyInventoryItemsInCategory(CItemVectorCalls* outItems, bool includeNonQuestPersistent)
{
    for (CItem_Methods* item = ItemsInCategory.first; item < ItemsInCategory.last; ++item) {
        if (item->NumberOfItems != 0) {
            if (includeNonQuestPersistent || item->IsQuestPersistent())
                outItems->PushBack(*item);
        }
    }
}