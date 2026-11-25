// NInventory::CCategory::AddItem(CThing& thing, bool select, long price) -- put one of the thing into the category: onto its type's stack (unless
//   full) or into the first empty slot, highlight that slot when the highlighted one is empty, and optionally select it and notify the
//   inventory; false when there is no room
// Leaf: ?AddItem@CCategory@NInventory@@QAE_NAAVCThing@@_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF5A4, 176 bytes, `ret 0Ch`; size-optimised TU with frame pointer.
// Name (NF90): Ego_r.pdb 0x0062239E + FableWin.pdb ?AddItem@CCategory@NInventory@@QAE_NAAVCThing@@_NJ@Z (the (CThing&, bool, long) overload); the
//   (long, bool, long, long) overload is 0x005BF060. The item type is CThing::GetDefGlobalIndex (FableWin.pdb, inline: the word at +0x8C, read twice).
// Callees: GetSlotWithItemsOfType 0x005BE85B / GetFirstEmptySlot 0x005BD96B (Ego_r private const), CItem::IsSlotFull 0x005BDE50 (twice),
//   CItem::AddItem(CThing&, long) 0x005BF556, SetSelectedItemIndex 0x005BC479, CTCInventoryBase vtable slot 33 (+0x84) OnItemSelected(long, bool&).
// Port (NF90, q11182 / q11183 binder, claimed through the lock): was the local CCategory_Retail model (leaf
//   ?AddItem@CCategory_Retail@@QAE_NPAVCThing@@_NJ@Z) binding both entry addresses under CCategory_Retail spellings; now on the 0x005BF060 headers
//   (NF90 opt-ins _CCATEGORY_ADDITEM_THING / _CITEM_ADDITEM_THING) + engine/CThing.h.
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
#define FABLE_RETAIL_CCATEGORY_ADDITEM_THING
#define FABLE_RETAIL_CCATEGORY_SETSELECTEDITEMINDEX
#define FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL
#define FABLE_RETAIL_NINVENTORY_CITEM_ADDITEM_THING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
bool CCategory::AddItem(CThing& thing, bool select, long price)
{
    long slot = GetSlotWithItemsOfType(thing.GetDefGlobalIndex());
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
    ItemsInCategory[slot].AddItem(thing, price);
    if (ItemsInCategory[HighlightedItemIndex].IsSlotEmpty())
        HighlightedItemIndex = slot;
    if (select) {
        SetSelectedItemIndex(slot);
        bool handled;
        PInventory->OnItemSelected(thing.GetDefGlobalIndex(), handled);
    }
    return true;
}
}
