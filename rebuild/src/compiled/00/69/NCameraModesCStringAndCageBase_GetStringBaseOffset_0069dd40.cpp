// NCameraModes::CStringAndCageBase::GetStringBaseOffset() const -- returns the zero vector
// Leaf: ?GetStringBaseOffset@CStringAndCageBase@NCameraModes@@MBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DD40, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 24 of NCameraModes::CSACCombat's vtable 0x012775B4 (the slot holds this body in 3 vtable(s), owner NCameraModes::CStringAndCageBase); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetStringBaseOffset(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetStringBaseOffset@CStringAndCageBase@NCameraModes@@MBE?AVC3DVector@@XZ; `ret` matches the signature.
// Was: NCameraModes::CStringAndCageBase::vfunc_24 (rebuild/src/compiled/00/69/NCameraModesCStringAndCageBase_vfunc24_0069dd40.cpp).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS_MORE
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_VSLOTS_MORE2
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_VSLOTS
#include "engine/NCameraModes_CStringAndCage.h"
namespace NCameraModes {
C3DVector CStringAndCageBase::GetStringBaseOffset() const
{
    return C3DVector(0.0f, 0.0f, 0.0f);
}
}
