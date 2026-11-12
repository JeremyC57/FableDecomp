// NCameraModes::CZTargetNew::GetDefaultObservedPositionZOffset() const -- returns the current value (CBase::GetObservedPositionZOffset, inline)
// Leaf: ?GetDefaultObservedPositionZOffset@CZTargetNew@NCameraModes@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871E90, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 20 of NCameraModes::CZTargetNew's vtable 0x0127754C (the slot holds this body in 1 vtable(s), owner NCameraModes::CZTargetNew); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetDefaultObservedPositionZOffset(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetDefaultObservedPositionZOffset@CZTargetNew@NCameraModes@@UBEMXZ; `ret` matches the signature.
// Was: NCameraModes::CZTargetNew::vfunc_20 (rebuild/src/compiled/00/87/gapscan_sub00871e90_00871e90.cpp).
// No shared header for NCameraModes::CZTargetNew (RTTI : NCameraModes::CBase): minimal local declaration of the real class.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
namespace NCameraModes {
class CZTargetNew : public CBase {
public:
    virtual float GetDefaultObservedPositionZOffset() const;
};
float CZTargetNew::GetDefaultObservedPositionZOffset() const
{
    return GetObservedPositionZOffset();
}
}
