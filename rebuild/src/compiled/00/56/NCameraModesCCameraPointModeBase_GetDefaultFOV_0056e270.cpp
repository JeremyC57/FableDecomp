// NCameraModes::CCameraPointModeBase::GetDefaultFOV() const -- returns FOV
// Leaf: ?GetDefaultFOV@CCameraPointModeBase@NCameraModes@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E270, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 18 of NCameraModes::CDead's vtable 0x0125C5F4 (the slot holds this body in 5 vtable(s), owner NCameraModes::CCameraPointModeBase); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetDefaultFOV(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetDefaultFOV@CCameraPointModeBase@NCameraModes@@UBEMXZ; `ret` matches the signature.
// Was: NCameraModes::CCameraPointModeBase::vfunc_18 (rebuild/src/compiled/00/56/gapscan_sub0056e270_0056e270.cpp).
// No shared header for NCameraModes::CCameraPointModeBase (RTTI : CBase): minimal local declaration of the real class with the
// Ego_r.pdb members 0x24 lower in retail (CBase is 0xa4 there, 0xc8 in Ego_r).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;
namespace NCameraModes {
class CCameraPointModeBase : public CBase {
public:
    virtual float GetDefaultFOV() const;
protected:
    CIntelligentPointer<const CThing> PCameraPoint;                 // +0xa4
    C3DVector Pos;                                                  // +0xac
    float FOV;                                                      // +0xb8
    bool CutInto;                                                   // +0xbc
    bool CutOutOf;                                                  // +0xbd
};
float CCameraPointModeBase::GetDefaultFOV() const
{
    return FOV;
}
}
