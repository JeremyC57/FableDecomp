// CTCPhysicsNavigator::OnCreate() -- CTCPhysicsStandard::OnCreate, then target = current position, no forced move, navigating, zero additional impulse
// Retry 10: RELOCATION_MATCH -- AdditionalImpulse zeroed through an inline C3DVector::Set(x, y, z) storing Z, Y, X (keeps the `xor eax,eax` after the Navigating store).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007268D0, length 69 bytes.
// Callees: CTCPhysicsStandard::OnCreate() @ 0x00724010 (qualified, non-virtual call).
// Shape: Position (+0x0c) is copied into PositionToNavigateTo (+0x89) through two `lea` bases (the implicit assignment of a POD
// C3DVector), ForcedMoveToPosSet is cleared (`and byte [esi+0ADh],0FEh`), Navigating set, and AdditionalImpulse zeroed Z, Y, X
// from one `xor eax,eax` register.
// Layout: Ego_r.pdb CTCPhysicsBase / CTCPhysicsStandard / CTCPhysicsNavigator (packed, Navigating +0x88 ... flags +0xad); the
// engine headers are flattened PODs without virtuals, so the hierarchy is modelled locally with the same offsets.
class C3DVector { public: float X, Y, Z; void Set(float x, float y, float z) { Z = z; Y = y; X = x; } };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CTCPhysicsBase { public:                        // 0x40 bytes
    virtual void v00();
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
    unsigned char   m_flags84, m_flags85;      // +0x84 / +0x85 physics bitfields
    void OnCreate();                           // 0x00724010
};

#pragma pack(push, 1)
struct CTCPhysicsNavigator : CTCPhysicsStandard {
    bool      Navigating;                      // +0x88
    C3DVector PositionToNavigateTo;            // +0x89
    C3DVector AdditionalImpulse;               // +0x95
    C3DVector ForcedMoveToPos;                 // +0xa1
    bool ForcedMoveToPosSet : 1;               // +0xad bit 0
    bool ForcedMoveToPosUsePhysics : 1;        // +0xad bit 1
    void OnCreate();
};
#pragma pack(pop)

void CTCPhysicsNavigator::OnCreate()
{
    CTCPhysicsStandard::OnCreate();
    PositionToNavigateTo = Position;
    ForcedMoveToPosSet = false;
    Navigating = true;
    AdditionalImpulse.Set(0.0f, 0.0f, 0.0f);
}