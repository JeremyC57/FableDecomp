// NInventory::CTCInventoryBase::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCInventoryBase@NInventory@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7D3B, thiscall, `ret`.
// Identity: slot 23 of CTCInventoryMagic's vtable 0x012668EC (defined here: the slot holds this body in 6 vtable(s), all of NInventory::CTCInventoryBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCInventoryBase@NInventory@@UBE_NXZ; `ret` matches the signature.
// Header: engine/NInventory_CTCInventoryBase.h (opt-ins as in the landed row NInventoryCTCInventoryBase_RemoveAllConfiscatedItems_005bf969.cpp).
#define FABLE_RETAIL_NINVENTORY_CTCINVENTORYBASE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#include "engine/NInventory_CTCInventoryBase.h"
bool NInventory::CTCInventoryBase::IsComponentSerialisable() const
{
    return true;
}
