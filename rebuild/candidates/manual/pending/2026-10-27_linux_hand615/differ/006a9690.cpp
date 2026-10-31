// CThingPlayerCreature::ResolveRelativeMovementAcceleration() -- turn the frame's requested relative movement acceleration into a world acceleration: normalise it (kept as the last relative acceleration), rotate it about Z by the control angle (the render camera's angle while the player uses the free camera, else the current game camera's control angle), scale it by the maximum acceleration; a blocking creature with a lean angle snaps its facing and drops its acceleration; a non-negligible result is added to MovementAcceleration and marks MovedByPlayer; the request is cleared
// Grade: DIFFER(450v450) -- 5 masked diff lines: the RotateXY `this` lea is scheduled before the spilled angle reload (retail: mov eax,[esp+0Ch]; push eax; lea ecx,[esp+14h]); everything else matches
// Leaf: ?ResolveRelativeMovementAcceleration@CThingPlayerCreature@@IAEXXZ
// Chunk 615: respelled with the PDB names (PeekWorld, PeekMyPlayer, GetCurrentRenderCamera by value + CCamera::GetAngleXY,
//   CAIGameCameraBase::GetControlAngleXY slot 7, C3DVector::RotateXY / AccurateNormalise, GetMaxAcceleration slot 87,
//   IsBlocking const, C3DAngles + CRightHandedSet(const C3DAngles&) + SetFacingAnglesFromRHSet slot 74, the PDB members
//   MovementAcceleration / RelativeMovementAcceleration / LastRelativeMovementAcceleration / LeanAngle); the MovedByPlayer
//   clear keeps the parked lever (a named unsigned char& to the bitfield byte gives the RMW `and byte ptr [esi+178h],0FEh`;
//   the real bitfield assignment gives load/and/store). Same 5 lines as the parked file, which used stand-in names. Tried:
//   the angle ternary inside the RotateXY argument (same lea order).
#include <math.h>
#define UNPROVEN_SLOT(n) virtual void UnprovenSlot##n();
#define UNPROVEN_SLOTS10(p) UNPROVEN_SLOT(p##0) UNPROVEN_SLOT(p##1) UNPROVEN_SLOT(p##2) UNPROVEN_SLOT(p##3) UNPROVEN_SLOT(p##4) \
    UNPROVEN_SLOT(p##5) UNPROVEN_SLOT(p##6) UNPROVEN_SLOT(p##7) UNPROVEN_SLOT(p##8) UNPROVEN_SLOT(p##9)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& AccurateNormalise();             // 0x00A14440
    void RotateXY(float angle);                 // 0x00A14040
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
};
extern const float g_NormaliseEpsilon;          // 0x0129BA3C (descriptive)

class C3DAngles {
public:
    float AngleXY, AngleYZ, AngleXZ;
    C3DAngles(float xy, float yz, float xz) : AngleXY(xy), AngleYZ(yz), AngleXZ(xz) { }
};
class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DAngles& angles);   // 0x00A229E0
};
class CCamera {
public:
    C3DVector       WorldPosition;
    CRightHandedSet RHSet;
    bool            HeightLocked;
    bool            ViewVecZLocked;
    unsigned long   FOVFlags;
    float           HorizontalFOV;
    float           VerticalFOV;
    float           Zoom;
    float GetAngleXY() const;                   // 0x00A0C380
};
class CAIGameCameraBase {
public:
    UNPROVEN_SLOT(0) UNPROVEN_SLOT(1) UNPROVEN_SLOT(2) UNPROVEN_SLOT(3) UNPROVEN_SLOT(4) UNPROVEN_SLOT(5) UNPROVEN_SLOT(6)
    virtual float GetControlAngleXY() const;    // slot 7 (+0x1c)
};
class CPlayer {
public:
    unsigned char _pad00[0x220];
    bool          UsingFreeCamera;              // +0x220 (engine/CPlayer.h)
};
class CWorld {
public:
    UNPROVEN_SLOT(0) UNPROVEN_SLOT(1) UNPROVEN_SLOT(2) UNPROVEN_SLOT(3) UNPROVEN_SLOT(4) UNPROVEN_SLOT(5)
    virtual CCamera GetCurrentRenderCamera() const;   // slot 6 (+0x18) 0x0049DA60
    unsigned char      _pad04[0x34 - 0x04];
    CAIGameCameraBase* PCurrentGameCamera;      // +0x34
    const CPlayer* PeekMyPlayer() const;        // 0x0049D9D0
};
class CTCPhysicsBase {
public:
    UNPROVEN_SLOTS10(0) UNPROVEN_SLOTS10(1) UNPROVEN_SLOTS10(2) UNPROVEN_SLOTS10(3) UNPROVEN_SLOTS10(4) UNPROVEN_SLOTS10(5)
    UNPROVEN_SLOT(60) UNPROVEN_SLOT(61) UNPROVEN_SLOT(62)
    virtual float GetFacingAngleXY() const;     // slot 63 (+0xfc)
    UNPROVEN_SLOT(64) UNPROVEN_SLOT(65) UNPROVEN_SLOT(66) UNPROVEN_SLOT(67) UNPROVEN_SLOT(68) UNPROVEN_SLOT(69)
    UNPROVEN_SLOT(70) UNPROVEN_SLOT(71) UNPROVEN_SLOT(72) UNPROVEN_SLOT(73)
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& set);   // slot 74 (+0x128)
};
class CThing {
public:
    UNPROVEN_SLOTS10(0) UNPROVEN_SLOTS10(1) UNPROVEN_SLOTS10(2) UNPROVEN_SLOTS10(3) UNPROVEN_SLOTS10(4) UNPROVEN_SLOTS10(5)
    UNPROVEN_SLOTS10(6) UNPROVEN_SLOTS10(7)
    UNPROVEN_SLOT(80) UNPROVEN_SLOT(81) UNPROVEN_SLOT(82) UNPROVEN_SLOT(83) UNPROVEN_SLOT(84) UNPROVEN_SLOT(85)
    UNPROVEN_SLOT(86)
    virtual float GetMaxAcceleration() const;   // slot 87 (+0x15c)
    unsigned char   _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
protected:
    const CWorld& PeekWorld() const;            // 0x004C7AB0
};
class CThingCreatureBase : public CThing {
public:
    bool IsBlocking() const;                    // 0x00664D00
};
class CThingPlayerCreature : public CThingCreatureBase {
public:
    unsigned char _pad64[0x178 - 0x64];
    unsigned char MovedByPlayerFlags;           // +0x178 bitfield group: MovedByPlayer = bit 0, Jumping = bit 1 (Ego_r uchar:1 each)
    unsigned char _pad179[0x180 - 0x179];
    C3DVector MovementAcceleration;             // +0x180
    unsigned char _pad18c[0x1b4 - 0x18c];
    C3DVector RelativeMovementAcceleration;     // +0x1b4
    C3DVector LastRelativeMovementAcceleration; // +0x1c0
    unsigned char _pad1cc[0x1fc - 0x1cc];
    float LeanAngle;                            // +0x1fc
protected:
    void ResolveRelativeMovementAcceleration();
};

void CThingPlayerCreature::ResolveRelativeMovementAcceleration()
{
    unsigned char& flags = MovedByPlayerFlags;
    flags &= ~1;
    C3DVector acceleration(RelativeMovementAcceleration.X, RelativeMovementAcceleration.Y, RelativeMovementAcceleration.Z);
    acceleration.AccurateNormalise();
    LastRelativeMovementAcceleration = acceleration;
    const CWorld& world = PeekWorld();
    float angle;
    if (world.PeekMyPlayer()->UsingFreeCamera)
        angle = world.GetCurrentRenderCamera().GetAngleXY();
    else
        angle = world.PCurrentGameCamera->GetControlAngleXY();
    acceleration.RotateXY(angle);
    acceleration *= GetMaxAcceleration();
    if (IsBlocking()) {
        if (fabs(LeanAngle) > g_NormaliseEpsilon) {
            LeanAngle = 0.0f;
            C3DAngles angles(GetPhysicsTC()->GetFacingAngleXY(), 0.0f, 0.0f);
            CRightHandedSet set(angles);
            GetPhysicsTC()->SetFacingAnglesFromRHSet(set);
        }
        MovementAcceleration.Z = 0.0f;
        MovementAcceleration.Y = 0.0f;
        MovementAcceleration.X = 0.0f;
    }
    if (acceleration.MagSq() > g_NormaliseEpsilon * g_NormaliseEpsilon) {
        MovementAcceleration += acceleration;
        MovedByPlayerFlags |= 1;
    }
    RelativeMovementAcceleration.Z = 0.0f;
    RelativeMovementAcceleration.Y = 0.0f;
    RelativeMovementAcceleration.X = 0.0f;
}
