// CTCPhysicsControlled::Move() -- one controlled physics step: honour a pending forced move (teleport, or fold it into the impulse), gather the things the impulse and velocity can reach, apply the impulse and step movement, then commit the position and update the additional impulse
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725C40, 499 bytes (+ int3 padding), plain `ret`.
// Callees: CTCPhysicsStandard::PerformForceUpdates 0x008E5670, GetPossibleInteractedWithList 0x008E57D0,
//   PerformStepBasedMovement 0x008E5880, UpdateIsMoving 0x008E5270, UpdateVelocityForImpassability 0x007243C0;
//   CTCPhysicsControlled::AdjustThingListForDebugHelpers 0x007254B0, PerformImpulseBasedMovement 0x00725500,
//   UpdateAdditionalImpulse 0x00725B70; virtual slots 30 SetPosition (+0x78), 97 UpdateOnSurface (+0x184),
//   98 GetPossibleCollidedWithArea (+0x188, C2DBoxI by hidden pointer) -- CTCPhysicsStandard vtable 0x01265F84;
//   free 0x00BFEA14 (the vector<CThing*> storage). Constants 0.5f 0x0122F59C, 4.5f 0x01266308.
// Layout: CTCPhysicsBase Position +0x0c, Velocity +0x28 (engine/CTCPhysicsBase.h); CTCPhysicsControlled ImpulseVelocity
//   +0x88, ForcedMoveToPos +0x94, ForcedMoveToPosSet/UsePhysics +0xa0 bits 0/1 (PDB offsets; retail CTCPhysicsStandard
//   base is the PDB 0x88). The owner thing's interface bits 0xA8/0xA9 are one `test ch,3` on m_presentBits[5].
// Shape: the forced teleport `goto`s the shared SetPosition tail (retail `jmp`); the impulse length is a squared local
//   with the sqrt in the call expression (retail spills the square, sqrt late); the margin is `0.5f` overridden by an
//   `if` (retail `fld 0.5; je; fstp; fld 4.5`); the owner is a named const local (otherwise every later scratch
//   register rotates by one).
#include <math.h>
#pragma intrinsic(sqrt)
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
};
class C2DBoxI { public: long Left; long Top; long Right; long Bottom; };
class CThing;
struct CThingPtrVector {                            // retail STLport vector<CThing*> (12 bytes)
    CThing** first;
    CThing** last;
    CThing** endOfStorage;
    CThingPtrVector() : first(0), last(0), endOfStorage(0) { }
    ~CThingPtrVector() { if (first) free(first); }
};
class CPhysicsDef;
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                // +0x20
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};
#define V4(n) virtual void v##n##a(); virtual void v##n##b(); virtual void v##n##c(); virtual void v##n##d();
struct CTCPhysicsControlled {
    V4(0) V4(1) V4(2) V4(3) V4(4) V4(5) V4(6)        // slots 0-27
    virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& pos); // slot 30 (+0x78)
    virtual void v31();
    V4(8) V4(9) V4(10) V4(11) V4(12) V4(13) V4(14) V4(15) V4(16) V4(17) V4(18) V4(19) V4(20) V4(21) V4(22) V4(23)   // 32-95
    virtual void v96();
    virtual void UpdateOnSurface(CThingPtrVector& things, C3DVector& position);   // slot 97 (+0x184)
    virtual C2DBoxI GetPossibleCollidedWithArea();  // slot 98 (+0x188)
    CThing* Owner;                                  // +0x04
    long _pad08;
    C3DVector Position;                             // +0x0c
    C3DVector OldPosition;                          // +0x18
    long OldPositionLastFrameSet;                   // +0x24
    C3DVector Velocity;                             // +0x28
    float Radius;                                   // +0x34
    CPhysicsDef* PhysicsDef;                        // +0x38
    unsigned char Moving : 1;                       // +0x3c bit 0
    unsigned char _pad3d[0x88 - 0x3d];
    C3DVector ImpulseVelocity;                      // +0x88
    C3DVector ForcedMoveToPos;                      // +0x94
    unsigned char ForcedMoveToPosSet : 1;           // +0xa0 bit 0
    unsigned char ForcedMoveToPosUsePhysics : 1;    // +0xa0 bit 1
    void PerformForceUpdates();                     // 0x008E5670
    void GetPossibleInteractedWithList(const C2DBoxI& area, CThingPtrVector& things, float speed);   // 0x008E57D0
    void UpdateVelocityForImpassability(const C3DVector& position, C3DVector& velocity);       // 0x007243C0
    void PerformStepBasedMovement(CThingPtrVector& things, C3DVector& position);              // 0x008E5880
    void UpdateIsMoving();                          // 0x008E5270
    void AdjustThingListForDebugHelpers(CThingPtrVector& things);                            // 0x007254B0
    void PerformImpulseBasedMovement(CThingPtrVector& things, C3DVector& position);           // 0x00725500
    void UpdateAdditionalImpulse();                 // 0x00725B70
    void Move();
};

void CTCPhysicsControlled::Move()
{
    PerformForceUpdates();
    C3DVector position(Position);
    if (ForcedMoveToPosSet) {
        ForcedMoveToPosSet = false;
        if (!ForcedMoveToPosUsePhysics) {
            position = ForcedMoveToPos;
            ImpulseVelocity.Z = 0.0f;
            ImpulseVelocity.Y = 0.0f;
            ImpulseVelocity.X = 0.0f;
            goto commit;
        }
        ImpulseVelocity += ForcedMoveToPos - Position;
    }
    {
        C2DBoxI area = GetPossibleCollidedWithArea();
        float impulseSpeedSquared = ImpulseVelocity.GetLengthSquared();
        CThingPtrVector things;
        float speed = Velocity.GetLength();
        float margin = 0.5f;
        const CThing* owner = Owner;
        if (owner->HasTC(0xa8) || owner->HasTC(0xa9))
            margin = 4.5f;
        GetPossibleInteractedWithList(area, things, (float)sqrt(impulseSpeedSquared) + margin + speed);
        AdjustThingListForDebugHelpers(things);
        PerformImpulseBasedMovement(things, position);
        ImpulseVelocity.Z = 0.0f;
        ImpulseVelocity.Y = 0.0f;
        ImpulseVelocity.X = 0.0f;
        UpdateVelocityForImpassability(position, Velocity);
        PerformStepBasedMovement(things, position);
        UpdateOnSurface(things, position);
        UpdateIsMoving();
    }
commit:
    SetPosition(position);
    UpdateAdditionalImpulse();
}