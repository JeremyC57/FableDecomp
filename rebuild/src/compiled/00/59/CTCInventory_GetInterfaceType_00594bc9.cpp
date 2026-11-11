// CTCInventory::GetInterfaceType() const -- the component's interface type, TCI_INVENTORY (0x11)
// Leaf: ?GetInterfaceType@CTCInventory@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594BC9, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCInventory's vtable 0x01250DE4 (the slot holds this body in 1 vtable(s), owner CTCInventory); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCInventory@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCInventory::vfunc_21 (rebuild/src/compiled/00/59/CTCInventory_vfunc21_00594bc9.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORY_VSLOTS
#define FABLE_RETAIL_CTCINVENTORY_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
ETCInterfaceType CTCInventory::GetInterfaceType() const
{
    return TCI_INVENTORY;
}
