// NCameraModes::CSimplePointBase::IsUseRightStickLookAround() const -- returns AllowLookAround
// Leaf: ?IsUseRightStickLookAround@CSimplePointBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873A80, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCameraModes::CLookAtFront's vtable 0x01277714 (the slot holds this body in 2 vtable(s), owner NCameraModes::CSimplePointBase); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsUseRightStickLookAround(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsUseRightStickLookAround@CSimplePointBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Was: NCameraModes::CSimplePointBase::vfunc_4 (rebuild/src/compiled/00/87/gapscan_sub00873a80_00873a80.cpp).
// No shared header for NCameraModes::CSimplePointBase (RTTI : CBase): minimal local declaration with the Ego_r.pdb members 0x24
// lower in retail.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
namespace NCameraModes {
class CSimplePointBase : public CBase {
public:
    virtual bool IsUseRightStickLookAround() const;
protected:
    C3DVector StartPos;                                             // +0xa4
    C3DVector Position;                                             // +0xb0
    C3DVector LookAtPos;                                            // +0xbc
    bool AllowLookAround;                                           // +0xc8
};
bool CSimplePointBase::IsUseRightStickLookAround() const
{
    return AllowLookAround;
}
}
