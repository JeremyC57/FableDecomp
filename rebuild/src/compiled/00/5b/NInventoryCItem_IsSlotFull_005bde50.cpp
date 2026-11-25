// NInventory::CItem::IsSlotFull() const -- false for an empty slot; else whether the count equals the item def's MaxNumberItems
//   (CInventoryItemDef +0x40, read through the counted def GetInventoryItemDef 0x005BD82E returns, released after the read)
// Leaf: ?IsSlotFull@CItem@NInventory@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDE50, length 70 bytes.
// Shape: the EBP frame with the 4-byte counted temporary, `this` in edi, `xor eax,eax ; cmp [edi+4],esi ; sete al`, `leave`.
// Identity (NF92, q11208): Ego_r.pdb 0x00620B75 + FableWin.pdb 0x0235F56C ?IsSlotFull@CItem@NInventory@@QBE_NXZ (public const); the
//   ported 0x005BF19C / 0x005BF060 / 0x005BF103 / 0x005BF5A4 bind this address under this spelling. Ego_d.pdb names the local
//   `max_number_of_items`. Was the July local model `CItem_Methods : CItem` (leaf ?IsSlotFull@CItem_Methods@@QAE_NXZ) with local counted
//   def / pointee models; now on engine/NInventory_CTCInventoryBase.h (NF90 opt-in _CITEM_ISSLOTFULL, NF92 opt-in _CITEM_GETINVENTORYITEMDEF),
//   engine/CDefPointer.h and engine/CInventoryItemDef.h.
#include "fable_compiler.h"
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: this row's object is byte-identical without it (size mode keeps the
//   leaf's frame; T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CLOTHING_DEFS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL
#define FABLE_RETAIL_NINVENTORY_CITEM_GETINVENTORYITEMDEF
#include "engine/CDefPointer.h"
#include "engine/CInventoryItemDef.h"
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
bool CItem::IsSlotFull() const
{
    if (IsSlotEmpty())
        return false;
    long max_number_of_items = GetInventoryItemDef()->MaxNumberItems;
    return NumberOfItems == max_number_of_items;
}
}
