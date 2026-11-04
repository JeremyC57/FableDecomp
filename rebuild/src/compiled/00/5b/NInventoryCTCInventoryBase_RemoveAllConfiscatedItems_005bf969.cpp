// NInventory::CTCInventoryBase::RemoveAllConfiscatedItems() -- forget every confiscated item stack without giving it back (ConfiscatedItems.clear())
// Leaf: ?RemoveAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BF969, 20 bytes: `push [ecx+0B0h] ; add ecx,0ACh ; push [ecx] ; call 0x005C1ECC ; ret`.
//   Size TU (unaligned bodies, `#pragma optimize("s", on)` as the landed same-TU slot-73 ReturnAllConfiscatedItems 0x005BF91A).
// Identity (two sources): slot 74 of the CTCInventoryClothing vtable 0x0124356C (rtti-vtables.tsv; the catalog's "vfunc_74"),
//   whose CTCInventoryWeapons override 0x005C2E68 (landed) calls this base body directly; Ego_r.pdb
//   ?RemoveAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ (0x00622A3D, right after ReturnAllConfiscatedItems 0x006229CE,
//   as retail 0x005BF969 follows 0x005BF91A).
// Shape: the inline STLport vector::clear() = erase(begin(), end()) with `this` = &ConfiscatedItems (both bounds pushed through
//   the vector pointer), erase out of line.
// Callees: std::vector<NInventory::CItem>::erase(CItem*, CItem*) 0x005C1ECC.
// Header: engine/NInventory_CTCInventoryBase.h (FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED, new).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::RemoveAllConfiscatedItems()
{
    ConfiscatedItems.clear();
}
}