// NInventory::CCategory::HighlightNextItem() -- moves the highlight cursor to the next slot (any slot when the def selects empty slots, wrapping or clamping at the end; else the next occupied slot) and tells the inventory when it changed
// Leaf: ?HighlightNextItem@CCategory@NInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD054, 163 bytes (plain `ret`); size-optimised TU.
// Callees: NInventory::CCategory::GetUniqueItemCountInCategory @ 0x005BC8AC and GetNextOccupiedSlot @ 0x005BCF61, the def's destroy (slot 1)
//   on the last release, NInventory::CTCInventoryBase virtual slot 36 (+0x90, OnItemHighlighted).
// Shape: SelectEmptySlots / WrapHighlightCursor are read through the by-value GetPDef() copy (retail AddRef `inc [def+4]` before and Release
//   after each read); the new index is stored before the UNSIGNED `>= size()` test.
// Name fix (agentNF89, q11168): Ego_r.pdb + FableWin.pdb ?HighlightNextItem@CCategory@NInventory@@QAEXXZ; it binds 0x005BC8AC as the const
//   ?GetUniqueItemCountInCategory@CCategory@NInventory@@QBEJXZ this pass defines. Moved off the row-local CCategory_Retail / CDefPointer / CDefRef /
//   CInventoryCategoryDef_Retail / CItemVector / CTCInventoryBase models onto engine/CCategory.h (PASS22; opt-ins CCATEGORY_UNIQUEITEMCOUNT,
//   CCATEGORY_GETNEXTOCCUPIEDSLOT and NF90's CCATEGORY_GETPDEF: FableWin.pdb ?GetPDef@CCategory@NInventory@@QBE?AV?$CDefPointer@$$CBVCInventoryCategoryDef@@@@XZ,
//   inline in retail), engine/_quarantine/CInventoryCategoryDef.h (NF90's CINVENTORYCATEGORYDEF_PARENTDEF) and NInventory_CTCInventoryBase.h (VTABLE).
//   CDEFPOINTER_RELEASE_POINTEE keeps retail's order (the byte load through the def before the Release `dec [def+4]`).
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
#define FABLE_RETAIL_CCATEGORY_GETNEXTOCCUPIEDSLOT
#define FABLE_RETAIL_CCATEGORY_GETPDEF
#define FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/_quarantine/CInventoryCategoryDef.h"
namespace NInventory {
void CCategory::HighlightNextItem()
{
    long previous = HighlightedItemIndex;
    if (GetPDef()->SelectEmptySlots) {
        if (HighlightedItemIndex + 1 < GetUniqueItemCountInCategory()) {
            ++HighlightedItemIndex;
            if ((unsigned long)HighlightedItemIndex >= ItemsInCategory.size()) {
                if (GetPDef()->WrapHighlightCursor)
                    HighlightedItemIndex = 0;
                else
                    HighlightedItemIndex = ItemsInCategory.size() - 1;
            }
        }
    } else {
        HighlightedItemIndex = GetNextOccupiedSlot(HighlightedItemIndex);
    }
    if (HighlightedItemIndex != previous)
        PInventory->OnItemHighlighted(ItemsInCategory[HighlightedItemIndex].ItemType);
}
}
