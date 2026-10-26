// NCameraModes::CBase::TransferPersistantBaseData(const CBase&) -- copies the persistent camera state from another mode: GoTo and LookAt position/velocity/cull flag, the two stick vectors, FOV, roll and Z offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084CF80, length 207 bytes (+1 int3 padding). No callees.
// Label: catalogued as CBase::TransferPersistantBaseData (Ego_r.pdb ?TransferPersistantBaseData@CBase@NCameraModes@@QAEXABV12@@Z).
// Shape: each CPhysicsState part goes through an inline helper; C3DVector has a user copy ctor and NO operator=, so the
// implicit assignment copies through `lea` pointer registers (`mov edx,esi ; mov ebx,edi`); a user operator= folds into
// plain offsets. GoTo is copied before LookAt and the stick vectors before the three floats.
// Layout (retail, see the landed dtor 0x0056E0E0): LookAt +0x10, GoTo +0x54 (CPhysicsState 0x30: Position +0, Velocity +0xc,
// CullPhysics +0x2c), CameraFOV +0x84, RollAngle +0x88, ObservedPositionZOffset +0x8c, RightStickVector +0x90, LeftStickVector +0x98.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class C2DVector { public: float X, Y; };
struct CAttractorList { void* _M_node; };          // list<pair<C3DVector,float>> (STLport, 4 bytes)
struct CPhysicsState {                             // retail 0x30 bytes
    C3DVector Position;                            // +0x00
    C3DVector Velocity;                            // +0x0c
    C3DVector Acceleration;                        // +0x18
    CAttractorList Attractors;                     // +0x24
    CAttractorList IncidentalAttractors;           // +0x28
    bool CullPhysics;                              // +0x2c
    void TransferPersistantData(const CPhysicsState& s)
    {
        Position = s.Position;
        Velocity = s.Velocity;
        CullPhysics = s.CullPhysics;
    }
};
class CGameCameraManager;
struct CSubComponentVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };

namespace NCameraModes {
class CBase { public:                                     // vtable 0x0124DD74
    void* vfptr;                                   // +0x00
    float DefaultFOV;                              // +0x04
    float DefaultRollAngle;                        // +0x08
    float DefaultObservedPositionZOffset;          // +0x0c
    CPhysicsState LookAt;                          // +0x10
    CGameCameraManager* CameraManager;             // +0x40
    void* PCameraModeDef;                          // +0x44 CDefPointer<CCameraModeDef>
    CSubComponentVector SubComponents;             // +0x48
    CPhysicsState GoTo;                            // +0x54
    float CameraFOV;                               // +0x84
    float RollAngle;                               // +0x88
    float ObservedPositionZOffset;                 // +0x8c
    C2DVector RightStickVector;                    // +0x90
    C2DVector LeftStickVector;                     // +0x98
    bool ViewOccluded;                             // +0xa0
    void TransferPersistantBaseData(const CBase& other);
};

void CBase::TransferPersistantBaseData(const CBase& other)
{
    GoTo.TransferPersistantData(other.GoTo);
    LookAt.TransferPersistantData(other.LookAt);
    RightStickVector = other.RightStickVector;
    LeftStickVector = other.LeftStickVector;
    CameraFOV = other.CameraFOV;
    RollAngle = other.RollAngle;
    ObservedPositionZOffset = other.ObservedPositionZOffset;
}
}