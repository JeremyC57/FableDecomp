// NCameraModes::CTrack::GoBehindPlayer(float) -- sets ZTargeting (the delay argument is unused)
// Leaf: ?GoBehindPlayer@CTrack@NCameraModes@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B820, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 30 of NCameraModes::CTrack's vtable 0x0125C50C (the slot holds this body in 1 vtable(s), owner NCameraModes::CTrack); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GoBehindPlayer(float) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GoBehindPlayer@CTrack@NCameraModes@@UAEXM@Z; `ret` matches the signature.
// Was: NCameraModes::CTrack::vfunc_30 (rebuild/src/compiled/00/87/unattributed_vfunc30_0087b820.cpp).
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
// No shared header for NCameraModes::CTrack (RTTI : CCameraPointModeBase): minimal local declaration with its Ego_r.pdb members.
class CTrack : public CCameraPointModeBase {
public:
    virtual void GoBehindPlayer(float);
protected:
    unsigned char _pad_0xc0[0x8];                                   // +0xc0 PTrack (Ego_r `CTrack *`), CurrentFractionAlongTrack
    bool ZTargeting;                                                // +0xc8
};
void CTrack::GoBehindPlayer(float)
{
    ZTargeting = true;
}
}
