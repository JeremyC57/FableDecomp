// CTCPhysicsBase::GetRHSet(float interpolation) -- the base physics body has no orientation of its own: return a copy of the default right-handed set
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1450, 60 bytes, CTCPhysicsBase vtable slot 71 (catalogued as vfunc_71; CTCPhysicsStandard
//   0x00723C80 / CTCPhysicsRigidBody 0x00759B40 are the GetRHSet overrides, the Standard one lerping OldRHSet->RHSet by the float).
// Callees: none. Global: the default CRightHandedSet at 0x0137F9B4 (24 bytes: Up, Forward).
// Layout: engine/CTCPhysicsBase.h for the class (no member read); PDB CRightHandedSet = { C3DVector Up; C3DVector Forward; }.
// Shape: C3DVector carries a user memberwise copy constructor and CRightHandedSet copies implicitly: the first float goes
//   through the x87 (`fld ; fstp`), the remaining five dwords through ecx/edx -- a POD C3DVector copies all six as dwords.
// Leaf: GetRHSet
#include "engine/CTCPhysicsBase.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CRightHandedSet { public:
    C3DVector Up;                           // +0x00
    C3DVector Forward;                      // +0x0c
    CRightHandedSet() { }
};

extern CRightHandedSet g_DefaultRHSet;      // 0x0137F9B4

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    CRightHandedSet GetRHSet(float interpolation) const;
};

CRightHandedSet CTCPhysicsBase_Methods::GetRHSet(float interpolation) const
{
    return g_DefaultRHSet;
}