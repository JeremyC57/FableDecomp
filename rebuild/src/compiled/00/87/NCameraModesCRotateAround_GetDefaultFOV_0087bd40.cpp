// NCameraModes::CRotateAround::GetDefaultFOV() const -- returns the current value (CBase::GetFOV, inline)
// Leaf: ?GetDefaultFOV@CRotateAround@NCameraModes@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087BD40, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 18 of NCameraModes::CRotateAround's vtable 0x0125C674 (the slot holds this body in 1 vtable(s), owner NCameraModes::CRotateAround); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetDefaultFOV(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetDefaultFOV@CRotateAround@NCameraModes@@UBEMXZ; `ret` matches the signature.
// Was: NCameraModes::CRotateAround::vfunc_18 (rebuild/src/compiled/00/87/gapscan_sub0087bd40_0087bd40.cpp).
// No shared header for NCameraModes::CRotateAround (RTTI : NCameraModes::CBase): minimal local declaration of the real class.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
namespace NCameraModes {
class CRotateAround : public CBase {
public:
    virtual float GetDefaultFOV() const;
};
float CRotateAround::GetDefaultFOV() const
{
    return GetFOV();
}
}
