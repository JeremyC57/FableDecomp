// NInventory::CCategory::GetItemInSlotWithIndex(long index) const -- the item type stored in slot `index` of the category (0 for a negative index)
// Leaf: ?GetItemInSlotWithIndex@CCategory@NInventory@@QBEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC9B0, length 24 bytes (`ret 4`): `test index ; jl -> xor eax,eax`, else
//   ItemsInCategory (+0x08) begin [index * 0x14] (CItem::ItemType +0x00).
// Callers: CTCInventoryTrade::SellHighlightedItem 0x005A3436 and BuyHighlightedItem 0x005A3581.
// Name fix (agentNF91, q11195): Ego_r.pdb (0x0061F7D9) + FableWin.pdb ?GetItemInSlotWithIndex@CCategory@NInventory@@QBEJJ@Z (public const,
//   returns long); engine/CCategory.h declares it at this address (FABLE_RETAIL_CCATEGORY_ITEMINSLOT, NF89). The old row was a __fastcall
//   global stand-in ?CCategory_GetItemInSlotWithIndex@@YIPAUCItem@@PAUCCategory@@HH@Z on a local CCategory / slot model; now the member on
//   the shared PASS22 NInventory::CCategory and NInventory::CItem (engine/NInventory_CTCInventoryBase.h CONFISCATED: ItemType +0x00 of the
//   0x14-byte CItem), the 0x005BC8AC recipe (size-optimised TU).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMINSLOT
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
long CCategory::GetItemInSlotWithIndex(long index) const
{
    if (index >= 0)
        return ItemsInCategory[index].ItemType;
    return 0;
}
}
