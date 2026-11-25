// NInventory::CTCInventoryBase::InitialiseInventoryWithItems(const std::vector<NInventory::CItem>& items) -- adds every saved item stack in `items` to the inventory
// Leaf: ?InitialiseInventoryWithItems@CTCInventoryBase@NInventory@@AAEXABV?$vector@VCItem@NInventory@@V?$allocator@VCItem@NInventory@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF8CB, length 79 bytes. Frame-pointer size TU: optimize("s", on)
// (`this` and the index spilled to the frame, `push 14h ; pop ebx` divisor).
// Callees: CTCInventoryBase::AddInventoryItem(const CItem&) @ 0x005BF4A3 (manifest "Inventory_AddItem"; the result is ignored).
// Retail NInventory::CItem is 0x14 bytes; the retail std::vector is {first, last, end} (12 bytes). size() is the signed
// element difference converted to unsigned (`jb`/`jbe` compares), re-read after every call.
// Names (NF95, 2026-11-19, queue q11240): self rename from the stand-in CTCInventoryBase_Retail to Ego_r.pdb / FableWin.pdb
//   ?InitialiseInventoryWithItems@CTCInventoryBase@NInventory@@AAEXABV?$vector@VCItem@NInventory@@...@Z (private; the landed caller 0x005BF9E3
//   binds it so), and the callee to ?AddInventoryItem@CTCInventoryBase@NInventory@@AAE_NABVCItem@2@@Z (Ego_r.pdb 0x00622247 / FableWin.pdb
//   0x0236BE37); moved onto engine/NInventory_CTCInventoryBase.h (PASS22 complete-methods block).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::InitialiseInventoryWithItems(const std::vector<CItem>& items)
{
    for (unsigned long i = 0; i < items.size(); ++i)
        AddInventoryItem(items[i]);
}
}
