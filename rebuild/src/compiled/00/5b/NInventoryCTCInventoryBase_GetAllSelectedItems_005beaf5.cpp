// NInventory::CTCInventoryBase::GetAllSelectedItems(std::vector<long>& items) const -- refill items with each category's highlighted item, in
//   category order
// Leaf: ?GetAllSelectedItems@CTCInventoryBase@NInventory@@IBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BEAF5, 98 bytes, `ret 4`; size-optimised TU with frame pointer.
// Name (NF90, q11185): Ego_r.pdb 0x006213AB ?GetAllSelectedItems@CTCInventoryBase@NInventory@@IBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
//   (protected const), right after GetBoughtPrice 0x00621385 (= retail 0x005BEAD5 -> 0x005BEAF5); the landed caller
//   CTCInventoryClothing::SetUpSelectedClothes 0x005B4795 already binds exactly this protected spelling. The old row defined the public QBE form.
// Callees: vector<long>::erase(first, last) 0x00440590 (clear() expanded to the out-of-line range erase), CCategory::GetHighlightedItem
//   0x005BC99C (Ego_r.pdb / FableWin.pdb, public const), vector<long>::push_back 0x00485FF3 (out of line; the item homed in a local).
// Port (NF90): was a local NInventory::CCategory / CTCInventoryBase / CRetailVector / std::vector<long> model; now on
//   engine/NInventory_CTCInventoryBase.h (ALL_ITEMS_CHAIN_PASS22 types Categories as std::vector<CCategory>; NF90 opt-in
//   FABLE_RETAIL_CTCINVENTORYBASE_GETALLSELECTEDITEMS) + engine/CCategory.h (NF90 opt-in FABLE_RETAIL_CCATEGORY_GETHIGHLIGHTEDITEM) +
//   stlport/fable_containers.h (VECTOR_ERASE_RANGE_OUTOFLINE, VECTOR_PUSHBACK_OUTOFLINE, VECTOR_INDEX_START: with begin()-based operator[] VC7.1
//   swaps the index and byte-offset registers, 100 v 98 bytes, grade 1; the old row also indexed _M_start).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_GETALLSELECTEDITEMS
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_GETHIGHLIGHTEDITEM
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_INDEX_START
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
void CTCInventoryBase::GetAllSelectedItems(std::vector<long>& items) const
{
    items.clear();
    for (unsigned long i = 0; i < Categories.size(); ++i)
        items.push_back(Categories[i].GetHighlightedItem());
}
}
