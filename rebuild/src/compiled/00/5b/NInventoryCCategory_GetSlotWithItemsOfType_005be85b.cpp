// NInventory::CCategory::GetSlotWithItemsOfType(long itemDefIndex) const -- the index of the slot holding the item type (only a non-full slot when
//   the category def lets items fill more than one slot), -1 when absent
// Leaf: ?GetSlotWithItemsOfType@CCategory@NInventory@@ABEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE85B, 86 bytes, `ret 4`; size-optimised TU with frame pointer (`push 14h ; pop ecx` divisor).
// Name (NF90, q11182): Ego_r.pdb 0x00620FEF + FableWin.pdb ?GetSlotWithItemsOfType@CCategory@NInventory@@ABEJJ@Z (private const), between
//   GetBoughtPrice 0x00620FCD and GetPerItemDataForItem 0x00621047 = retail 0x005BE839 / 0x005BE85B / 0x005BE8B1 (NF88); the ported callers
//   0x005BF1D7 (NF88), 0x005BF19C, 0x005BF16E and 0x005BF060 (NF90) bind this spelling.
// Body: std::find_if(begin, end, CIsSameItemType(itemDefIndex, GetPDef()->AllowItemsToFillMoreThanOneSlot)) (the 0x005BE839 find_if 0x005C1921
//   with the global CIsSameItemType predicate, Ego_r layout {ItemType, OnlyFindNonFullSlots}); GetPDef (FableWin.pdb, inline) returns a counted
//   CDefPointer copy, a temporary of the full expression: retail AddRefs it (`inc [esi+4]`, null-checked) before and releases it
//   (`dec [esi+4] ; jne ; call [eax+4]`) after the find_if call, then turns the iterator into an index.
// Port (NF90): was the local CCategory_Retail / CItemVector / CDefPointer / CDefRef / CInventoryCategoryDef_Retail model (leaf
//   ?GetSlotWithItemsOfType@CCategory_Retail@@QAEJJ@Z, binding find_if as ?FindMatchingItem@@YIPAUCItem_Methods@@PAU1@0USItemMatcher@@@Z); now on
//   engine/NInventory_CTCInventoryBase.h + engine/CCategory.h (PASS22, NF88 _ITEMQUERIES, NF90 opt-in FABLE_RETAIL_CCATEGORY_GETPDEF) +
//   engine/_quarantine/CInventoryCategoryDef.h (NF90 opt-in FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF: the def on CParentDefClassBase) +
//   stlport/fable_algorithm.h (find_if declared), the 0x005BE839 define set.
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
#define FABLE_RETAIL_CCATEGORY_GETPDEF
#define FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/_quarantine/CInventoryCategoryDef.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
long CCategory::GetSlotWithItemsOfType(long itemDefIndex) const
{
    std::vector<CItem>::const_iterator it = std::find_if(ItemsInCategory.begin(), ItemsInCategory.end(),
        CIsSameItemType(itemDefIndex, GetPDef()->AllowItemsToFillMoreThanOneSlot));
    if (it != ItemsInCategory.end())
        return it - ItemsInCategory.begin();
    return -1;
}
}
