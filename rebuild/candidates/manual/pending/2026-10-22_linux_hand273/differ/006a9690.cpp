// CThingPlayerCreature::ResolveRelativeMovementAcceleration() -- turn the frame's requested relative acceleration into a world velocity change: normalise it (kept as the direction), rotate it by the control angle (camera angle for the main player's free camera, else the clock's), scale it by the maximum speed; a blocking creature with a pending turn snaps its facing and stops; a non-negligible acceleration is added to the velocity and marks the creature as accelerating; the request is cleared
// Grade: DIFFER(458v450) -- flag clear compiles to load/and/store via dl where retail uses a read-modify-write `and byte ptr [esi+0x178],0xfe`; the RotateAboutZ this lea is emitted before the float push (retail after); tried a bool bitfield, a ternary angle, an inline ClearAccelerating helper
// Leaf: ResolveRelativeMovementAcceleration
// Callees: C3DVector::Normalise @ 0x00A14440, CThing::GetWorld @ 0x004C7AB0, CWorld::GetMainPlayer @ 0x0049D9D0, the world's
// virtual slot 6 (+0x18, camera by scratch pointer), Math_ComputeAngleXY @ 0x00A0C380, the world clock's slot 7 (+0x1c),
// C3DVector::RotateAboutZ @ 0x00A14040, own virtual slot 87 (+0x15c, max speed), CThingCreatureBase::IsBlocking @ 0x00664D00,
// the physics component's slots 63 (+0xfc, facing angle) and 74 (+0x128, set right-handed set), CRightHandedSet(const C3DVector&)
// @ 0x00A229E0. Global: the normalise epsilon @ 0x0129BA3C.
// Layout (retail, descriptive): CThing PhysicsTC +0x60; CThingPlayerCreature flag byte +0x178 (bit 0 accelerating), velocity
// +0x180, requested relative acceleration +0x1b4, its direction +0x1c0, pending turn +0x1fc; CWorld clock +0x34; CPlayer flag
// byte +0x220.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                           // 0x00A14440
    void RotateAboutZ(float angle);             // 0x00A14040
    void operator*=(float scale)
    {
        X *= scale;
        Y *= scale;
        Z *= scale;
    }
    void operator+=(const C3DVector& other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
    }
    float MagSq() const { return Z * Z + Y * Y + X * X; }
    void Clear() { X = Y = Z = 0.0f; }
};
extern const float g_NormaliseEpsilon;          // 0x0129BA3C

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& eulerAngles);   // 0x00A229E0
};
struct SCameraScratch { unsigned char _b[0x38]; };
struct CCameraView {
    float ComputeAngleXY();                     // 0x00A0C380
};
struct CWorldClock {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual float GetControlAngle();            // slot 7 (+0x1c)
};
struct CPlayer {
    unsigned char _pad00[0x220];
    bool FreeCamera;                            // +0x220
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CCameraView* GetCameraView(SCameraScratch* scratch);   // slot 6 (+0x18)
    unsigned char _pad04[0x34 - 0x04];
    CWorldClock* Clock;                         // +0x34
    CPlayer* GetMainPlayer() const;             // 0x0049D9D0
};
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCPhysicsBase {
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY() const;     // slot 63 (+0xfc)
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void SetRHSet(const CRightHandedSet& set);   // slot 74 (+0x128)
};

class CThingPlayerCreature {
public:
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6) FILL10(7)
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85();
    virtual void v86();
    virtual float GetMaxSpeed();                // slot 87 (+0x15c)
    unsigned char _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad64[0x178 - 0x64];
    unsigned char Flags178;                     // +0x178 (bit 0 accelerating)
    unsigned char _pad179[0x180 - 0x179];
    C3DVector Velocity;                         // +0x180
    unsigned char _pad18c[0x1b4 - 0x18c];
    C3DVector RelativeAcceleration;             // +0x1b4
    C3DVector RelativeAccelerationDirection;    // +0x1c0
    unsigned char _pad1cc[0x1fc - 0x1cc];
    float PendingTurn;                          // +0x1fc
    CWorld* GetWorld();                         // 0x004C7AB0
    bool IsBlocking();                          // 0x00664D00
    void ResolveRelativeMovementAcceleration();
};

void CThingPlayerCreature::ResolveRelativeMovementAcceleration()
{
    Flags178 &= ~1;
    C3DVector acceleration(RelativeAcceleration.X, RelativeAcceleration.Y, RelativeAcceleration.Z);
    acceleration.Normalise();
    RelativeAccelerationDirection = acceleration;
    CWorld* world = GetWorld();
    float angle;
    if (world->GetMainPlayer()->FreeCamera) {
        SCameraScratch scratch;
        angle = world->GetCameraView(&scratch)->ComputeAngleXY();
    }
    else
        angle = world->Clock->GetControlAngle();
    acceleration.RotateAboutZ(angle);
    acceleration *= GetMaxSpeed();
    if (IsBlocking()) {
        if (fabs(PendingTurn) > g_NormaliseEpsilon) {
            PendingTurn = 0.0f;
            C3DVector angles(PhysicsTC->GetFacingAngleXY(), 0.0f, 0.0f);
            CRightHandedSet set(angles);
            PhysicsTC->SetRHSet(set);
        }
        Velocity.Z = 0.0f;
        Velocity.Y = 0.0f;
        Velocity.X = 0.0f;
    }
    if (acceleration.MagSq() > g_NormaliseEpsilon * g_NormaliseEpsilon) {
        Velocity += acceleration;
        Flags178 |= 1;
    }
    RelativeAcceleration.Z = 0.0f;
    RelativeAcceleration.Y = 0.0f;
    RelativeAcceleration.X = 0.0f;
}
