// NInventory::CTCInventoryBase::SelectHighlightedItem(bool& selected) -- clears `selected`, then lets the selected category select its highlighted item when SelectedCategoryIndex is a valid index
// Leaf: ?SelectHighlightedItem@CTCInventoryBase@NInventory@@QAEXAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDD04, length 53 bytes; size TU (`push 2Ch ; pop ebx` divisor inside a push/pop ebx pair).
// Callees: CCategory::SelectHighlightedItem(bool&) @ 0x005BD9B2.
// Retail CTCInventoryBase: no engine header; Categories at +0x20 (0x2C-byte elements), SelectedCategoryIndex at +0x70.
// Shape: the signed `>= 0` test (`jl`) then the unsigned size compare (`jae`) against the re-computed element count
// is `idx >= 0 && (unsigned long)idx < size()`; the reference argument stays in edi for the store and the forward.
// Names (NF95, 2026-11-19, queue q11234): self rename from the stand-in CTCInventoryBase_Retail to Ego_r.pdb 0x00620A83 / FableWin.pdb
//   0x02368DDE ?SelectHighlightedItem@CTCInventoryBase@NInventory@@QAEXAA_N@Z, and the call to Ego_r.pdb 0x006208AF / FableWin.pdb 0x0236333E
//   ?SelectHighlightedItem@CCategory@NInventory@@QAEXAA_N@Z (0x005BD9B2, landed under that leaf); moved onto engine/NInventory_CTCInventoryBase.h
//   (the landed 0x005BDCB4 recipe) with engine/CCategory.h FABLE_RETAIL_CCATEGORY_SELECTHIGHLIGHTEDITEM.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_UNIQUEITEMCOUNT
#define FABLE_RETAIL_CCATEGORY_SELECTHIGHLIGHTEDITEM
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::SelectHighlightedItem(bool& close_input_process)
{
    close_input_process = false;
    if (SelectedCategoryIndex >= 0 && (unsigned long)SelectedCategoryIndex < Categories.size())
        Categories[SelectedCategoryIndex].SelectHighlightedItem(close_input_process);
}
}
