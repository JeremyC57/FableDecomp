// CTCPhysicsBase::GetBoundingInfo(C3DVector& centre, float& radius) -- the bounding sphere in world space: the physics mesh's sphere offset by the position when there is one, otherwise the position raised by the thing's radius
// Leaf: ?GetBoundingInfo@CTCPhysicsBase@@QBEXAAVC3DVector@@AAM@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF640, 139 bytes.
// Callees: virtual slot 93 (+0x174) of CTCPhysicsBase (0x006B13B0 in the CTCPhysicsBase vtable), returning the local bounding
//   physics mesh info (PDB CThingPhysicsMeshInfo: ToCentre +0x08, BoundingRadius +0x14) or null.
// Layout: engine/CTCPhysicsBase.h: Position +0x0c, Radius +0x34 (modelled locally to declare the virtual slot).
// Shape: the mesh arm `centre = C3DVector(...)` built in the frame and copied through integer registers (3-float ctor only); the
//   radius is stored first in the fallback arm, the position copy then `centre.Z += radius` read back through the reference.
#include "engine/C3DVector.h"
class CThingPhysicsMeshInfo { public:
    unsigned char _pad00[0x08];
    C3DVector ToCentre;                         // +0x08
    float     BoundingRadius;                   // +0x14
};
// NF59 (round 7, queue q10522): the component is the retail engine/CTCPhysicsBase.h (FABLE_RETAIL_CTCPHYSICSBASE: the const virtual
//   GetPhysicsMeshInfo slot 93 and ?GetBoundingInfo@CTCPhysicsBase@@QBEXAAVC3DVector@@AAM@Z, Ego_r.pdb 0x006EC5B0, const with reference
//   parameters); the local CTCPhysicsBase model (placeholder slots v000..v092) defined it non-const (QAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"

void CTCPhysicsBase::GetBoundingInfo(C3DVector& centre, float& radius) const
{
    const CThingPhysicsMeshInfo* sphere = GetPhysicsMeshInfo();
    if (sphere) {
        centre = C3DVector(sphere->ToCentre.X + Position.X, sphere->ToCentre.Y + Position.Y, sphere->ToCentre.Z + Position.Z);
        radius = sphere->BoundingRadius;
        return;
    }
    radius = Radius;
    centre = Position;
    centre.Z += radius;
}