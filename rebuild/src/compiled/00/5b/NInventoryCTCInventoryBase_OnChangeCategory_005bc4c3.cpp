// NInventory::CTCInventoryBase::OnChangeCategory() -- does nothing
// Leaf: ?OnChangeCategory@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BC4C3, thiscall, `ret`.
// Identity: slot 37 of CTCInventoryAbilities's vtable 0x01256AD4 (defined here: the slot holds this body in 2 vtable(s), all of NInventory::CTCInventoryBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts OnChangeCategory(void) at this slot; Ego_r/FableWin name set: ?OnChangeCategory@CTCInventoryBase@NInventory@@UAEXXZ; `ret` matches the signature.
// Header: engine/NInventory_CTCInventoryBase.h (opt-ins as in the landed row NInventoryCTCInventoryBase_RemoveAllConfiscatedItems_005bf969.cpp).
#define FABLE_RETAIL_NINVENTORY_CTCINVENTORYBASE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#include "engine/NInventory_CTCInventoryBase.h"
void NInventory::CTCInventoryBase::OnChangeCategory()
{
}
