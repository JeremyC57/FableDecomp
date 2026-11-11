// NCameraModes::CStringAndCage::GetAutoGoBehindTime() const -- returns AutoGoBehindTime
// Leaf: ?GetAutoGoBehindTime@CStringAndCage@NCameraModes@@MBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DED0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 29 of NCameraModes::CStringAndCage's vtable 0x0125C404 (the slot holds this body in 1 vtable(s), owner NCameraModes::CStringAndCage); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetAutoGoBehindTime(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAutoGoBehindTime@CStringAndCage@NCameraModes@@MBEMXZ; `ret` matches the signature.
// Was: NCameraModes::CStringAndCage::vfunc_29 (rebuild/src/compiled/00/69/gapscan_sub0069ded0_0069ded0.cpp).
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
float CStringAndCage::GetAutoGoBehindTime() const
{
    return AutoGoBehindTime;
}
}
