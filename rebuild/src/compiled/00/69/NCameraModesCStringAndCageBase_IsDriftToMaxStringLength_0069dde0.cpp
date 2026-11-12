// NCameraModes::CStringAndCageBase::IsDriftToMaxStringLength() const -- returns false
// Leaf: ?IsDriftToMaxStringLength@CStringAndCageBase@NCameraModes@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DDE0, thiscall, `ret`.
// Identity: slot 31 of NCameraModes::CStringAndCage's vtable 0x0125C404 (defined here: the slot holds this body in 2 vtable(s), all of NCameraModes::CStringAndCageBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsDriftToMaxStringLength(void) at this slot; Ego_r/FableWin name set: ?IsDriftToMaxStringLength@CStringAndCageBase@NCameraModes@@MBE_NXZ; `ret` matches the signature.
// Header: engine/NCameraModes_CStringAndCageBase.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS
#include "engine/NCameraModes_CStringAndCageBase.h"
bool NCameraModes::CStringAndCageBase::IsDriftToMaxStringLength() const
{
    return false;
}
