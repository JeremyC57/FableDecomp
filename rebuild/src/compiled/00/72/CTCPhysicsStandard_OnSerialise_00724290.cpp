// CTCPhysicsStandard::OnSerialise(CPersistContext& context, bool loading) -- serialise the physics base, then the right-handed set as six floats (Forward XYZ, Up XYZ, default 0)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724290, length 300 bytes (+ int3 padding), `ret 8`. Vtable 0x01265F84
// (CTCPhysicsStandard) slot 1, the CTCBase::OnSerialise(CPersistContext&, bool) override; the catalog labels the row
// "CTCPhysicsRigidBody::OnSerialise" (propagated: CTCPhysicsRigidBody's own slot-1 copy is 0x0075A350, the same body with
// RHSet at +0x68).
// Callees: CTCPhysicsBase::OnSerialise @ 0x006B08F0 (qualified base call), CPersistContext::Transfer<float> @ 0x00410620
// `(name, value&, const default&)`. Keys 0x01266154 "RHSetForwardX" .. 0x01266110 "RHSetUpZ".
// Shape: Up and Forward are copied into two frame C3DVector locals (declared Up first; Forward gets the lower frame words)
// through C3DVector's user copy constructor: the implicit POD copy goes through a `mov ecx,edi` carrier instead of retail's
// direct `[ebx+50h]` loads with `lea edi,[ebx+50h]` / `add ebx,5Ch` kept as the write-back bases. Each 0.0f default is a
// literal bound to the const reference and homed in the dead `loading` argument slot; Forward and Up are assigned back
// (implicit operator=) after the six transfers.
// Layout: Ego_r.pdb CTCPhysicsStandard (RHSet +0x50 = Up, Forward +0x5c); modelled locally like the landed UpdateOldRHSet
// 0x00723930 / ResetRHSet 0x00723980 (engine/CTCPhysicsBase.h is a flattened POD with no virtuals).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: float 0x00410620
};

class CTCPhysicsBase { public:                        // 0x40 bytes
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

struct CTCPhysicsStandard : CTCPhysicsBase {   // 0x88 bytes
    void*           PPhysicsMeshInfo;          // +0x40
    C3DVector       OffsetVector3D;            // +0x44
    CRightHandedSet RHSet;                     // +0x50
    CRightHandedSet OldRHSet;                  // +0x68
    long            RHSetLastFrameSet;         // +0x80
    unsigned char   m_flags[4];                // +0x84 sixteen bool:1 flags
    void OnSerialise(CPersistContext& context, bool loading);
};

void CTCPhysicsStandard::OnSerialise(CPersistContext& context, bool loading)
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