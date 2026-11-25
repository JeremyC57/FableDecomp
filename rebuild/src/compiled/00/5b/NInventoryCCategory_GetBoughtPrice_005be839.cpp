// NInventory::CCategory::GetBoughtPrice(long itemDefIndex) const -- the AverageBoughtPrice of the category's first stack of that item type (no end check)
// Leaf: ?GetBoughtPrice@CCategory@NInventory@@QBEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE839, 34 bytes, `ret 4`; size-optimised TU with a frame pointer (`optimize("s",on)`:
//   the CIsSameItemType temporary lives at [ebp-8], its flag byte homed at [ebp-4] and pushed as a dword).
// Name: Ego_r.pdb 0x00620FCD ?GetBoughtPrice@CCategory@NInventory@@QBEJJ@Z, first of the CCategory block in the inventory TU (then
//   GetSlotWithItemsOfType 0x00620FEF / retail 0x005BE85B, GetPerItemDataForItem 0x00621047 / retail 0x005BE8B1); the tail-jump target of
//   NInventory::CTCInventoryBase::GetBoughtPrice 0x005BEAD5 (q11153 / q11157).
// Callee: std::find_if<const NInventory::CItem*, CIsSameItemType> 0x005C1921 (__fastcall: first in ecx, last in edx, the 8-byte predicate on the stack).
// Port (NF88, round 7, q11157): the local CCategory_Retail / CItem_Methods / CItemVector / SItemMatcher models are gone: the row is on
//   engine/NInventory_CTCInventoryBase.h (NInventory::CItem, CONFISCATED) + engine/CCategory.h (the PASS22 NInventory::CCategory, NF88 opt-in
//   FABLE_RETAIL_CCATEGORY_ITEMQUERIES: the member and CIsSameItemType) + stlport/fable_algorithm.h (find_if declared, FIND_IF_CALLED_PASS23).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
long CCategory::GetBoughtPrice(long itemDefIndex) const
{
    return std::find_if(ItemsInCategory.begin(), ItemsInCategory.end(), CIsSameItemType(itemDefIndex, false))->AverageBoughtPrice;
}
}
