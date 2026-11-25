// NInventory::CCategory::AddItem(long itemDefIndex, bool select, long price, long count) -- put items of a type into the category: onto its
//   stack (unless full) or into the first empty slot, highlight that slot when the highlighted one is empty, and optionally select it and
//   notify the inventory; false when there is no room
// Leaf: ?AddItem@CCategory@NInventory@@QAE_NJ_NJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF060, 163 bytes, `ret 10h`; size-optimised TU with frame pointer.
// Name (NF90): Ego_r.pdb 0x00621E43 + FableWin.pdb ?AddItem@CCategory@NInventory@@QAE_NJ_NJJ@Z (the (long, bool, long, long) overload), first of
//   the TU run AddItem 0x00621E43 / AddInventoryItem 0x00621EE3 / UpdateItemStatsForThing 0x00621F4C / IsSpaceInCategoryForItem 0x00621F79
//   = retail 0x005BF060 / 0x005BF103 / 0x005BF16E / 0x005BF19C (Ego_r deltas 0xA0 / 0x69 / 0x2D v retail 163 / 107 / 46). The other catalog
//   AddItem, 0x005BF5A4, is the (CThing&, bool, long) overload (Ego_r 0x0062239E).
// Callees: GetSlotWithItemsOfType 0x005BE85B / GetFirstEmptySlot 0x005BD96B (Ego_r private const), CItem::IsSlotFull 0x005BDE50 (twice: the
//   stack test, then the common test), CItem::AddItem 0x005BEC92, SetSelectedItemIndex 0x005BC479, CTCInventoryBase vtable slot 33 (+0x84)
//   OnItemSelected(long, bool&) with an uninitialised local flag.
// Port (NF90, q11182 / q11183 binder): was the local CCategory_Retail / CDefPointer / CTCInventoryBase (33 placeholder slots) model (leaf
//   ?AddItem@CCategory_Retail@@QAE_NJ_NJJ@Z); now on engine/NInventory_CTCInventoryBase.h (CONFISCATED CItem, VTABLE, NF90 opt-ins
//   _CITEM_ISSLOTFULL / _CITEM_ADDITEM) + engine/CCategory.h (PASS22, NF88 _ITEMQUERIES, NF89's _ADDITEM declaration, NF90 _SETSELECTEDITEMINDEX).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CCATEGORY_ADDITEM
#define FABLE_RETAIL_CCATEGORY_SETSELECTEDITEMINDEX
#define FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL
#define FABLE_RETAIL_NINVENTORY_CITEM_ADDITEM
#include "fable_string.h"
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
bool CCategory::AddItem(long itemDefIndex, bool select, long price, long count)
{
    long slot = GetSlotWithItemsOfType(itemDefIndex);
    if (slot != -1) {
        if (ItemsInCategory[slot].IsSlotFull())
            return false;
    } else {
        slot = GetFirstEmptySlot();
        if (slot == -1)
            return false;
    }
    if (ItemsInCategory[slot].IsSlotFull())
        return false;
    ItemsInCategory[slot].AddItem(itemDefIndex, count, price);
    if (ItemsInCategory[HighlightedItemIndex].IsSlotEmpty())
        HighlightedItemIndex = slot;
    if (select) {
        SetSelectedItemIndex(slot);
        bool handled;
        PInventory->OnItemSelected(itemDefIndex, handled);
    }
    return true;
}
}
