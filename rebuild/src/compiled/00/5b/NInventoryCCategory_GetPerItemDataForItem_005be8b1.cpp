// NInventory::CCategory::GetPerItemDataForItem(long itemDefIndex) const -- the per-item data of the category's first stack of that item type (no end check)
// Leaf: ?GetPerItemDataForItem@CCategory@NInventory@@QBEPBVCPerItemData@2@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE8B1, 34 bytes, `ret 4`; size-optimised TU with a frame pointer (`optimize("s",on)`:
//   the CIsSameItemType temporary at [ebp-8]); returns the stack's PPerItemData data pointer (+0x08).
// Name: Ego_r.pdb 0x00621047 ?GetPerItemDataForItem@CCategory@NInventory@@QBEPBVCPerItemData@2@J@Z (TU order: GetBoughtPrice 0x00620FCD,
//   GetSlotWithItemsOfType 0x00620FEF, this, then CTCInventoryBase::GetPerItemDataForItem 0x00621169 = retail 0x005BE8D3, which tail-jumps here;
//   q11156). The catalog's GetPerItemData is a made-up short form.
// Callee: std::find_if<const NInventory::CItem*, CIsSameItemType> 0x005C1921.
// Shape: the CCountedPointer's public Data member is read directly (`mov eax,[eax+8]`); through Get() VC7.1 materialises the member
//   address first (`add eax,8 ; mov eax,[eax]`, 36 bytes, NF88 grade 1).
// Port (NF88, round 7, q11156): the local CCategory_Retail / CItem_Methods / CItemVector / SItemMatcher models are gone: the row is on
//   engine/NInventory_CTCInventoryBase.h (NInventory::CItem, CONFISCATED; NInventory::CPerItemData, PERITEMDATA) + engine/CCategory.h (the
//   PASS22 NInventory::CCategory, NF88 opt-in FABLE_RETAIL_CCATEGORY_ITEMQUERIES) + stlport/fable_algorithm.h (find_if declared).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
const CPerItemData* CCategory::GetPerItemDataForItem(long itemDefIndex) const
{
    return std::find_if(ItemsInCategory.begin(), ItemsInCategory.end(), CIsSameItemType(itemDefIndex, false))->PPerItemData.Data;
}
}
