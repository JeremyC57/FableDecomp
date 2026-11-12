// NCameraModes::CStringAndCage::IsAutoGoBehind() const -- returns AutoGoBehind
// Leaf: ?IsAutoGoBehind@CStringAndCage@NCameraModes@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DEC0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 28 of NCameraModes::CStringAndCage's vtable 0x0125C404 (the slot holds this body in 1 vtable(s), owner NCameraModes::CStringAndCage); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsAutoGoBehind(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsAutoGoBehind@CStringAndCage@NCameraModes@@MBE_NXZ; `ret` matches the signature.
// Was: NCameraModes::CStringAndCage::vfunc_28 (rebuild/src/compiled/00/69/gapscan_sub0069dec0_0069dec0.cpp).
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
bool CStringAndCage::IsAutoGoBehind() const
{
    return AutoGoBehind;
}
}
