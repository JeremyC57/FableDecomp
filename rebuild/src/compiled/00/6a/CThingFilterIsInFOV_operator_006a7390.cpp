// CThingFilter_IsInFOV::operator()(const CThing& thing) const -- fetch the centre (and radius) of the thing's physics bounding info and test that point against the field of view (the C3DVector overload)
// Leaf: ??RCThingFilter_IsInFOV@@QBE_NABVCThing@@@Z
// Name-fix (agentNF55, round 7): Ego_r.pdb / FableWin.pdb ??RCThingFilter_IsInFOV@@QBE_NABVCThing@@@Z (the const CThing& overload; the
//   landed caller 0x008090E0 is Ego_r ??RCThingFilter_IsInTargetingRangeAndFOV@?A0x58ed6966@@QBE_NABVCThing@@@Z and passes its
//   reference, which selects this overload), re-authored on the shared engine/CThingFilter_IsInFOV.h (the local model is gone).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7390, length 47 bytes (+ int3 padding).
// Callees: CTCPhysicsBase::GetBoundingInfo @ 0x006AF640 (Ego_r.pdb const, C3DVector& / float& out-parameters),
// CThingFilter_IsInFOV::operator()(const C3DVector&) @ 0x006A7270.
// Layout: PDB CThingFilter_IsInFOV { C3DVector FromPos; C3DVector Direction; float CosFOV; float FOV; bool AngleTest3D; };
// retail CThing PhysicsTC +0x60 (no null test here).
// Shape: the centre is a 12-byte frame local; the unused radius out-value is homed in the dead `thing` slot
// (`lea eax,[esp+14h]`).
#include "engine/C3DVector.h"

class CTCPhysicsBase { public:
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640 Ego_r.pdb ?GetBoundingInfo@CTCPhysicsBase@@QBEXAAVC3DVector@@AAM@Z (NF55)
};

#include "engine/CThing.h"

#include "engine/CThingFilter_IsInFOV.h"

bool CThingFilter_IsInFOV::operator()(const CThing& thing) const
{
    C3DVector centre;
    float radius;
    thing.GetPhysicsTC()->GetBoundingInfo(centre, radius);
    return (*this)(centre);
}