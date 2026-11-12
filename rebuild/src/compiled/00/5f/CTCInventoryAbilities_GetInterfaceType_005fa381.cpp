// CTCInventoryAbilities::GetInterfaceType() const -- the component's interface type, TCI_HERO_ABILITIES (0x6F)
// Leaf: ?GetInterfaceType@CTCInventoryAbilities@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FA381, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCInventoryAbilities's vtable 0x01256AD4 (the slot holds this body in 1 vtable(s), owner CTCInventoryAbilities); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCInventoryAbilities@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCInventoryAbilities::vfunc_21 (rebuild/src/compiled/00/5f/CTCInventoryAbilities_vfunc21_005fa381.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYABILITIES_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/CTCInventoryAbilities.h"
ETCInterfaceType CTCInventoryAbilities::GetInterfaceType() const
{
    return StatGetInterfaceType();
}
