// CTCPhysicsLight::IsAffectedByGravity() const -- returns AffectedByGravity
// Leaf: ?IsAffectedByGravity@CTCPhysicsLight@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723440, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 48 of CTCPhysicsLight's vtable 0x01265DFC (the slot holds this body in 1 vtable(s), owner CTCPhysicsLight); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts IsAffectedByGravity(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsAffectedByGravity@CTCPhysicsLight@@UBE_NXZ; `ret` matches the signature.
// Was: CTCPhysicsLight::vfunc_48 (rebuild/src/compiled/00/72/CTCPhysicsLight_vfunc48_00723440.cpp).
// No shared header for CTCPhysicsLight (RTTI .?AVCTCPhysicsLight@@ : CTCPhysicsBase): minimal local declaration of the real class
// with its one Ego_r.pdb member (AffectedByGravity, bool : 1 at +0x40).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
class CTCPhysicsLight : public CTCPhysicsBase {
public:
    virtual bool IsAffectedByGravity() const;
protected:
    bool AffectedByGravity : 1;                                     // +0x40
};
bool CTCPhysicsLight::IsAffectedByGravity() const
{
    return AffectedByGravity;
}
