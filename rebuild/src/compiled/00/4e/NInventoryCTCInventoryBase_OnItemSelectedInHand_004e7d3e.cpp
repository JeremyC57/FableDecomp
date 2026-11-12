// NInventory::CTCInventoryBase::OnItemSelectedInHand(CThing&, bool&) -- does nothing
// Leaf: ?OnItemSelectedInHand@CTCInventoryBase@NInventory@@UAEXAAVCThing@@AA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7D3E, thiscall, `ret 8`.
// Identity: slot 34 of CTCInventoryMagic's vtable 0x012668EC (defined here: the slot holds this body in 10 vtable(s), all of NInventory::CTCInventoryBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts OnItemSelectedInHand(CThing &, bool &) at this slot; Ego_r/FableWin name set: ?OnItemSelectedInHand@CTCInventoryBase@NInventory@@UAEXAAVCThing@@AA_N@Z; `ret` matches the signature.
// Header: engine/NInventory_CTCInventoryBase.h (opt-ins as in the landed row NInventoryCTCInventoryBase_RemoveAllConfiscatedItems_005bf969.cpp).
class CThing;
#define FABLE_RETAIL_NINVENTORY_CTCINVENTORYBASE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#include "engine/NInventory_CTCInventoryBase.h"
void NInventory::CTCInventoryBase::OnItemSelectedInHand(CThing&, bool&)
{
}
