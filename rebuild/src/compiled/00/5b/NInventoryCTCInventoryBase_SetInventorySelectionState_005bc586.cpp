// NInventory::CTCInventoryBase::SetInventorySelectionState(NInventory::CTCInventoryBase::EInventorySelectionState) -- does nothing
// Leaf: ?SetInventorySelectionState@CTCInventoryBase@NInventory@@UAEXW4EInventorySelectionState@12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BC586, thiscall, `ret 4`.
// Identity: slot 46 of CTCInventoryMagic's vtable 0x012668EC (defined here: the slot holds this body in 10 vtable(s), all of NInventory::CTCInventoryBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts SetInventorySelectionState(NInventory::CTCInventoryBase::EInventorySelectionState) at this slot; Ego_r/FableWin name set: ?SetInventorySelectionState@CTCInventoryBase@NInventory@@UAEXW4EInventorySelectionState@12@@Z; `ret` matches the signature.
// Header: engine/NInventory_CTCInventoryBase.h (opt-ins as in the landed row NInventoryCTCInventoryBase_RemoveAllConfiscatedItems_005bf969.cpp).
#define FABLE_RETAIL_NINVENTORY_CTCINVENTORYBASE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#include "engine/NInventory_CTCInventoryBase.h"
void NInventory::CTCInventoryBase::SetInventorySelectionState(NInventory::CTCInventoryBase::EInventorySelectionState)
{
}
