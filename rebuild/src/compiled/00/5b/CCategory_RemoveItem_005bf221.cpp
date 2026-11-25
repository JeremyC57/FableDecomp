// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
// NInventory::CCategory::RemoveItem(long itemDefIndex, long count) -- removes a number of items from the stack matching the item type (no end check)
// Leaf: ?RemoveItem@CCategory@NInventory@@QAEXJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF221, length 41 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: std::find_if<NInventory::CItem*, CIsSameItemType> @ 0x005C190B (__fastcall, the 8-byte predicate pushed by value),
//   NInventory::CItem::RemoveItem(long) @ 0x005BF00B. Callers: CTCInventoryTrade::RemoveItemFromInventory 0x0059C747 (tail jump) and
//   NInventory::CTCInventoryBase::RemoveItemFromInventory 0x005BF3C6 (both landed, binding this spelling).
// Frame-pointer body in the size TU (`optimize("s", on)`): the predicate temporary lives at [ebp-8]. The found item is a named local so
//   `count` is pushed after the find call (a single expression hoists that push above it).
// Name fix (NF96 part 2, q11458): Ego_r.pdb 0x00621FFD + FableWin.pdb 0x0236130C ?RemoveItem@CCategory@NInventory@@QAEXJJ@Z (engine/CCategory.h
//   FABLE_RETAIL_CCATEGORY_REMOVEITEM declares it). The old row defined ?RemoveItem@CCategory_Retail@@QAEXJJ@Z on a local CCategory_Retail /
//   CItemVector / SItemMatcher / CItem_Methods view and bound the made-up ?FindMatchingItem_005c190b@@ and ?RemoveItem@CItem_Methods@@QAEXJ@Z;
//   it now sits on the shared headers (the define set of the landed GetSlotWithItemsOfType 0x005BE85B: the PASS22 retail CCategory layout,
//   CIsSameItemType, the out-of-line STLport find_if) and binds 0x005C190B / 0x005BF00B by their landed spellings (overlay opt-in
//   FABLE_RETAIL_NINVENTORY_CITEM_REMOVEITEM).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CCATEGORY_REMOVEITEM
#define FABLE_RETAIL_NINVENTORY_CITEM_REMOVEITEM
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "engine/NInventory_CTCInventoryBase.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
void CCategory::RemoveItem(long itemDefIndex, long count)
{
    CItem* item = std::find_if(ItemsInCategory.begin(), ItemsInCategory.end(), CIsSameItemType(itemDefIndex, false));
    item->RemoveItem(count);
}
}
