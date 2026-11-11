// CTCInventoryExperience::GetInterfaceType() const -- the component's interface type, TCI_INVENTORY_EXPERIENCE (0x16)
// Leaf: ?GetInterfaceType@CTCInventoryExperience@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A238, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCInventoryExperience's vtable 0x0124F11C (the slot holds this body in 1 vtable(s), owner CTCInventoryExperience); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCInventoryExperience@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCInventoryExperience::vfunc_21 (rebuild/src/compiled/00/57/CTCInventoryExperience_vfunc21_0057a238.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_MENULIST
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryExperience.h"
ETCInterfaceType CTCInventoryExperience::GetInterfaceType() const
{
    return TCI_INVENTORY_EXPERIENCE;
}
