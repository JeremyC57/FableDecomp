// NInventory::CItem::AddInventoryItem(const CItem& item) -- merge another stack into this slot: an empty slot takes the other item wholesale;
//   otherwise add as many as still fit and re-average the bought price over the new count
// Leaf: ?AddInventoryItem@CItem@NInventory@@QAEXABV12@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDDE4, length 108 bytes.
// Callees: CCountedPointer<NInventory::CPerItemData>::operator=(const CCountedPointer&) 0x005C077F (out of line in this TU:
//   FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY), CItem::GetMaximumNumberOfItems 0x005BD931 (landed).
// Pragmas: the CItem module's frame-pointer size TU.
// Shape: the room left is a frame local so that std::min can return a reference to it (`cmp [ecx],eax ; mov eax,ecx ; jl ; lea eax,[ebp-4]`);
//   the new count is stored before the `idiv` that averages the price.
// Identity (NF92, q11214): Ego_r.pdb 0x00620B0A + FableWin.pdb ?AddInventoryItem@CItem@NInventory@@QAEXABV12@@Z (public); the ported 0x005BF103
//   binds it so; Ego_d.pdb names the parameter `item` and the inner-scope locals total_bought_price / number_of_extra_items_adding /
//   number_of_items_theres_space_for. Was a local NInventory::CItem / CPerItemDataPointer model (same mangled name); now on
//   engine/NInventory_CTCInventoryBase.h (NF90 opt-in _CITEM_ADDINVENTORYITEM, NF92 opt-in _CITEM_GETMAXIMUMNUMBEROFITEMS) and
//   engine/CCountedPointer.h.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY
#define FABLE_RETAIL_NINVENTORY_CITEM_ADDINVENTORYITEM
#define FABLE_RETAIL_NINVENTORY_CITEM_GETMAXIMUMNUMBEROFITEMS
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
void CItem::AddInventoryItem(const CItem& item)
{
    if (IsSlotEmpty()) {
        NumberOfItems = item.NumberOfItems;
        ItemType = item.ItemType;
        PPerItemData = item.PPerItemData;
        AverageBoughtPrice = item.AverageBoughtPrice;
    } else {
        long number_of_items_theres_space_for = GetMaximumNumberOfItems() - NumberOfItems;
        long number_of_extra_items_adding = std::min(number_of_items_theres_space_for, item.NumberOfItems);
        long total_bought_price = item.AverageBoughtPrice * number_of_extra_items_adding + AverageBoughtPrice * NumberOfItems;
        NumberOfItems = number_of_extra_items_adding + NumberOfItems;
        AverageBoughtPrice = total_bought_price / NumberOfItems;
    }
}
}
