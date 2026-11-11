// NCameraModes::CSnapToPoint::IsTrackObservedThing() const -- returns TrackThing
// Leaf: ?IsTrackObservedThing@CSnapToPoint@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E3A0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 28 of NCameraModes::CSnapToPoint's vtable 0x0124DE5C (the slot holds this body in 1 vtable(s), owner NCameraModes::CSnapToPoint); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsTrackObservedThing(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsTrackObservedThing@CSnapToPoint@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Was: NCameraModes::CSnapToPoint::vfunc_28 (rebuild/src/compiled/00/56/gapscan_sub0056e3a0_0056e3a0.cpp).
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
// No shared header for NCameraModes::CSnapToPoint (RTTI : CCameraPointModeBase): minimal local declaration.
class CSnapToPoint : public CCameraPointModeBase {
public:
    virtual bool IsTrackObservedThing() const;
protected:
    bool TrackThing;                                                // +0xc0
};
bool CSnapToPoint::IsTrackObservedThing() const
{
    return TrackThing;
}
}
