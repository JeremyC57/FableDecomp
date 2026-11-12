// NCameraModes::CStringAndCageBase::IsAutoGoBehind() const -- returns false
// Leaf: ?IsAutoGoBehind@CStringAndCageBase@NCameraModes@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DDA0, thiscall, `ret`.
// Identity: slot 28 of NCameraModes::CStringAndCageBase's vtable 0x0125C37C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CStringAndCageBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsAutoGoBehind(void) at this slot; Ego_r/FableWin name set: ?IsAutoGoBehind@CStringAndCageBase@NCameraModes@@MBE_NXZ; `ret` matches the signature.
// Header: engine/NCameraModes_CStringAndCageBase.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS
#include "engine/NCameraModes_CStringAndCageBase.h"
bool NCameraModes::CStringAndCageBase::IsAutoGoBehind() const
{
    return false;
}
