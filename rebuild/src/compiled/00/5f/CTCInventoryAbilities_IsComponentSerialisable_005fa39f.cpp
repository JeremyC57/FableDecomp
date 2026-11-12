// CTCInventoryAbilities::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCInventoryAbilities@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FA39F, thiscall, `ret`.
// Identity: slot 23 of CTCInventoryAbilities's vtable 0x01256AD4 (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryAbilities or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCInventoryAbilities@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCInventoryAbilities.h (opt-ins as in the landed row CTCInventoryAbilities_GetPrioritisedExpressionDefs_005eed8c.cpp).
#define FABLE_RETAIL_CTCINVENTORYABILITIES_VSLOTS
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
bool CTCInventoryAbilities::IsComponentSerialisable() const
{
    return true;
}
