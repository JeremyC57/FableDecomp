// CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to, CPhysicsCollideResponse& response) const -- dispatch a moving-sphere collision test on the other body's physics-mesh type: swept sphere v mesh (normal optionally flipped by a global switch), sphere v cylinder, or sphere v sphere at the end point (reporting the start point)
// Leaf: HasCollidedWithThingFromTo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00880730, 195 bytes (`ret 10h`). CTCPhysicsBase vtable 0x0125D2C4 slot 90 (catalog
//   "vfunc_90"; PDB ?HasCollidedWithThingFromTo@CTCPhysicsBase@@UBE_NABV1@ABVC3DVector@@1AAVCPhysicsCollideResponse@@@Z).
// Callees: IsPhysicsMeshTypeMesh() const @ 0x006AFD30 (manifest "CCombatSequenceBase::IsInterruptable" is propagated),
//   IsPhysicsMeshTypeCylinder() const @ 0x006B0170, HasSphereMeshCollidedFromTo @ 0x00880640, HasSphereCylinderCollidedAt
//   @ 0x008802B0, HasSphereSphereCollidedAt @ 0x008803E0 (all PDB `IBE_NABV1@...` const members). Globals: the flip switch byte
//   @ 0x013B86EC (unnamed in the PDB dump; named descriptively). Floats: -1.0f @ 0x0122DEE0, 1.0f @ 0x0122DED8.
// Layout: Ego_r.pdb CPhysicsCollideResponse {CollidePos +0x00, CollideNormal +0x0c, Friction +0x18} (0x1c).
// Shape: the scale is a named float from a ternary (one shared x87 value, `fld st(0) ; fmul` per component) applied through an
//   inline `C3DVector::operator*=`; the cylinder test is a forwarded return; the sphere hit copies `from` with the implicit
//   assignment (three dword moves); every `return false` shares the final `xor al,al` exit.
class C3DVector { public:
    float X, Y, Z;
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
class CPhysicsCollideResponse { public:
    C3DVector CollidePos;                               // +0x00
    C3DVector CollideNormal;                            // +0x0c
    float     Friction;                                 // +0x18
};
extern bool g_flipMeshCollideNormals_013b86ec;          // 0x013B86EC
class CTCPhysicsBase { public:
    virtual void v000();
    bool IsPhysicsMeshTypeMesh() const;                 // 0x006AFD30
    bool IsPhysicsMeshTypeCylinder() const;             // 0x006B0170
    bool HasSphereMeshCollidedFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                     CPhysicsCollideResponse& response) const;                    // 0x00880640
    bool HasSphereCylinderCollidedAt(const CTCPhysicsBase& other, const C3DVector& pos,
                                     CPhysicsCollideResponse& response) const;                    // 0x008802B0
    bool HasSphereSphereCollidedAt(const CTCPhysicsBase& other, const C3DVector& pos,
                                   CPhysicsCollideResponse& response) const;                      // 0x008803E0
    bool HasCollidedWithThingFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                    CPhysicsCollideResponse& response) const;
};

bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase& other, const C3DVector& from, const C3DVector& to,
                                                CPhysicsCollideResponse& response) const
{
    if (other.IsPhysicsMeshTypeMesh()) {
        if (HasSphereMeshCollidedFromTo(other, from, to, response)) {
            float scale = g_flipMeshCollideNormals_013b86ec ? -1.0f : 1.0f;
            response.CollideNormal *= scale;
            return true;
        }
        return false;
    }
    if (other.IsPhysicsMeshTypeCylinder())
        return HasSphereCylinderCollidedAt(other, to, response);
    if (HasSphereSphereCollidedAt(other, to, response)) {
        response.CollidePos = from;
        return true;
    }
    return false;
}