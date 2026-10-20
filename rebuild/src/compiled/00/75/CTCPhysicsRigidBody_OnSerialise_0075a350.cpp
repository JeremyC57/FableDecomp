// CTCPhysicsRigidBody::OnSerialise(CPersistContext& context, bool loading) -- serialise the physics base, then the right-handed set as six floats (Forward XYZ, Up XYZ, default 0)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075A350, length 300 bytes (+ int3 padding), `ret 8`. Vtable 0x0126996C
// (CTCPhysicsRigidBody) slot 1 (catalog "CTCPhysicsRigidBody::vfunc_1"), the CTCBase::OnSerialise(CPersistContext&, bool)
// override; the same body as CTCPhysicsStandard::OnSerialise 0x00724290 with RHSet at +0x68.
// Callees: CTCPhysicsBase::OnSerialise @ 0x006B08F0 (qualified base call; CTCPhysicsRigidBody derives straight from
// CTCPhysicsBase), CPersistContext::Transfer<float> @ 0x00410620 `(name, value&, const default&)`. Keys 0x01266154
// "RHSetForwardX" .. 0x01266110 "RHSetUpZ" (shared with the standard body).
// Shape: Up and Forward are copied into two frame C3DVector locals through C3DVector's user copy constructor (the implicit
// POD copy goes through a carrier register instead of retail's direct `[ebx+68h]` loads with `lea edi` / `add ebx,74h`
// kept as the write-back bases); each 0.0f default is a literal bound to the const reference and homed in the dead
// `loading` argument slot; Forward and Up are assigned back after the six transfers.
// Layout: Ego_r.pdb CTCPhysicsRigidBody has RHSet at +0x74 after three 16-byte std::vectors at +0x40; retail STLport
// vectors are 12 bytes, so retail RHSet sits at +0x68 (as in the landed vfunc_75 0x00759A00). engine/CTCPhysicsRigidBody.h
// follows the PDB offsets and has no virtuals, so the class is modelled locally.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CRetailVector { void* m_first; void* m_last; void* m_end; };   // retail STLport vector, 12 bytes

struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
};

struct CTCPhysicsBase {                        // 0x40 bytes
    virtual void v00();
    virtual void OnSerialise(CPersistContext& context, bool loading);   // slot 1; CTCPhysicsBase's body 0x006B08F0
    void*     m_tcBase[2];                     // +0x04 CTCBase body
    C3DVector Position;                        // +0x0c
    C3DVector OldPosition;                     // +0x18
    long      OldPositionLastFrameSet;         // +0x24
    C3DVector Velocity;                        // +0x28
    float     Radius;                          // +0x34
    void*     PhysicsDef;                      // +0x38 CDefPointer<CPhysicsDef const>::Object
    bool Moving : 1;                           // +0x3c bit 0
    bool OldMoving : 1;                        // +0x3c bit 1
};

struct CTCPhysicsRigidBody : CTCPhysicsBase {
    CRetailVector   CollisionNormal;           // +0x40 std::vector<C3DVector>
    CRetailVector   CollidingVertexIndex;      // +0x4c std::vector<long>
    CRetailVector   Collidors;                 // +0x58 std::vector<CThing*>
    void*           PPhysicsMeshInfo;          // +0x64
    CRightHandedSet RHSet;                     // +0x68
    CRightHandedSet OldRHSet;                  // +0x80
    void OnSerialise(CPersistContext& context, bool loading);
};

void CTCPhysicsRigidBody::OnSerialise(CPersistContext& context, bool loading)
{
    CTCPhysicsBase::OnSerialise(context, loading);
    C3DVector up = RHSet.Up;
    C3DVector forward = RHSet.Forward;
    context.Transfer("RHSetForwardX", forward.X, 0.0f);
    context.Transfer("RHSetForwardY", forward.Y, 0.0f);
    context.Transfer("RHSetForwardZ", forward.Z, 0.0f);
    context.Transfer("RHSetUpX", up.X, 0.0f);
    context.Transfer("RHSetUpY", up.Y, 0.0f);
    context.Transfer("RHSetUpZ", up.Z, 0.0f);
    RHSet.Forward = forward;
    RHSet.Up = up;
}