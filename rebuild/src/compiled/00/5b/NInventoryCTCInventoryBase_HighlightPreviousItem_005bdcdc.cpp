// CTCInventoryBase::HighlightPreviousItem() -- while the inventory is in item-selection state (1) and no category slide is running, plays the highlight sound and moves the selected category's highlight to the previous item
// Leaf: ?HighlightPreviousItem@CTCInventoryBase@NInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDCDC, length 40 bytes; size TU. The catalog label "HighlightNextItem" is propagated from the byte-twin
// 0x005BDCB4: this body tail-jumps CCategory::HighlightPreviousItem and is reached from the "up" input handler 0x005BDD8C.
// Callees: CTCInventoryBase sound helper @ 0x005BD33C (member, no arguments; manifest "PlaySoundForMeshAnimation"),
// CCategory::HighlightPreviousItem @ 0x005BD0F7 (tail jump on &Categories[SelectedCategoryIndex]).
// Retail CTCInventoryBase (was: no engine header): PDB layout minus 4 bytes per preceding std::vector:
// InventorySelectionState +0x1C, Categories +0x20 (0x2C-byte elements), SelectedCategoryIndex +0x70, CategoryMoving +0x89.
// Names (NF95, 2026-11-19, queue q11228): self rename from the stand-in CTCInventoryBase_Retail to Ego_r.pdb / FableWin.pdb
//   ?HighlightPreviousItem@CTCInventoryBase@NInventory@@QAEXXZ (the PASS22 complete-methods block declares it beside HighlightNextItem);
//   moved onto engine/NInventory_CTCInventoryBase.h, the landed 0x005BDCB4 recipe. The sound helper binds the PDB
//   ?PlayInventoryMovementSound@CTCInventoryBase@NInventory@@ABEXXZ and the tail jump ?HighlightPreviousItem@CCategory@NInventory@@QAEXXZ
//   (opt-in FABLE_RETAIL_CCATEGORY_HIGHLIGHTPREVIOUSITEM).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_UNIQUEITEMCOUNT
#define FABLE_RETAIL_CCATEGORY_HIGHLIGHTPREVIOUSITEM
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::HighlightPreviousItem()
{
    if (InventorySelectionState == INVENTORY_SELECTION_STATE_ITEMS && !CategoryMoving) {
        PlayInventoryMovementSound();
        Categories[SelectedCategoryIndex].HighlightPreviousItem();
    }
}
}
