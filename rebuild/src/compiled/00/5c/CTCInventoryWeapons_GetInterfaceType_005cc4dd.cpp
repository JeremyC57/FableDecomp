// CTCInventoryWeapons::GetInterfaceType() const -- the component's interface type, TCI_INVENTORY_WEAPONS (0x13)
// Leaf: ?GetInterfaceType@CTCInventoryWeapons@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CC4DD, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCInventoryWeapons's vtable 0x01253FF4 (the slot holds this body in 1 vtable(s), owner CTCInventoryWeapons); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCInventoryWeapons@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCInventoryWeapons::vfunc_21 (rebuild/src/compiled/00/5c/CTCInventoryWeapons_vfunc21_005cc4dd.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
ETCInterfaceType CTCInventoryWeapons::GetInterfaceType() const
{
    return StatGetInterfaceType();
}
