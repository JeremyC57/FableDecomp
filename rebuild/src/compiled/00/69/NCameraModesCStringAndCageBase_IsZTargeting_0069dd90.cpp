// NCameraModes::CStringAndCageBase::IsZTargeting() const -- returns false
// Leaf: ?IsZTargeting@CStringAndCageBase@NCameraModes@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DD90, thiscall, `ret`.
// Identity: slot 27 of NCameraModes::CSACCombat's vtable 0x012775B4 (defined here: the slot holds this body in 3 vtable(s), all of NCameraModes::CStringAndCageBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsZTargeting(void) at this slot; Ego_r/FableWin name set: ?IsZTargeting@CStringAndCageBase@NCameraModes@@MBE_NXZ; `ret` matches the signature.
// Header: engine/NCameraModes_CStringAndCageBase.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS
#include "engine/NCameraModes_CStringAndCageBase.h"
bool NCameraModes::CStringAndCageBase::IsZTargeting() const
{
    return false;
}
