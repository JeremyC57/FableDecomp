// CTCCreatureFlyInCircles::UpdateFlight(C3DVector& position, C3DVector& velocity) -- one step of circling flight: run the radius and speed ramps, bank by the transition progress (or a fixed quarter turn when strafing), move along the banked velocity, pull the position back onto the circle around the centre with a gentle height ease, and point the velocity along the flat step taken, banked again
// Leaf: ?UpdateFlight@CTCCreatureFlyInCircles@@IAEXAAVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DD270, 437 bytes, `ret 8`.
// Callees: CCreatureActionBase::GetProgressRatio 0x006925E0 (catalog "operator==" / landed "GetMovementRatio" are
//   propagated), C3DVector::RotateAboutZ 0x00A14040, C3DVector::AccurateNormalise @ 0x00A14440; constants -0.25 0x01230C8C,
//   1.0 0x0122DED8, 0.1 0x0122EE58.
// Layout: CTCCreatureFlyInCircles (Ego_r.pdb): Creature +0x0c, Centre +0x10, Speed +0x1c, DeltaSpeed +0x20,
//   SpeedChangeTicks +0x24, CurrentRadius +0x28, DeltaRadius +0x2c, RadiusChangeTicks +0x30, Mode +0x34,
//   Transitioning +0x38; CThingCreatureBase PCurrentAction +0x100 (retail).
// Shape: straight-line source with inline C3DVector operators; the bank lives in the dead position-argument slot.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector& AccurateNormalise();                                        // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
    void RotateAboutZ(float angle);                          // 0x00A14040
};

class CCreatureActionBase { public:
    float GetProgressRatio();                                // 0x006925E0
};
class CThingCreatureBase { public:
    unsigned char        _pad00[0x100];
    CCreatureActionBase* PCurrentAction;                     // +0x100 (CCountedPointer<CCreatureActionBase> data)
};

enum EFlyMode { FLY_MODE_STRAFE, FLY_MODE_LOOP, FLY_MODE_BREATHE_FIRE };

class CTCCreatureFlyInCircles {
public:
    void*               __vftable;                           // +0x00
    void*               ParentThing;                         // +0x04 (CTCBase)
    unsigned char       _pad08[0x0c - 0x08];
    CThingCreatureBase* Creature;                            // +0x0c
    C3DVector           Centre;                              // +0x10
    float               Speed;                               // +0x1c
    float               DeltaSpeed;                          // +0x20
    long                SpeedChangeTicks;                    // +0x24
    float               CurrentRadius;                       // +0x28
    float               DeltaRadius;                         // +0x2c
    long                RadiusChangeTicks;                   // +0x30
    EFlyMode            Mode;                                // +0x34
    bool                Transitioning;                       // +0x38
    void UpdateFlight(C3DVector& position, C3DVector& velocity);
};

void CTCCreatureFlyInCircles::UpdateFlight(C3DVector& position, C3DVector& velocity)
{
    C3DVector oldPosition = position;
    float bank = 0.0f;
    if (RadiusChangeTicks) {
        CurrentRadius += DeltaRadius;
        --RadiusChangeTicks;
    }
    if (SpeedChangeTicks) {
        Speed += DeltaSpeed;
        --SpeedChangeTicks;
    }
    if (Transitioning) {
        CCreatureActionBase* action = Creature->PCurrentAction;
        if (action) {
            float progress = action->GetProgressRatio();
            if (Mode == FLY_MODE_STRAFE)
                bank = progress * -0.25f;
            else
                bank = (1.0f - progress) * -0.25f;
        }
    } else if (Mode == FLY_MODE_STRAFE)
        bank = -0.25f;
    float height = position.Z;
    velocity.Z = 0.0f;
    velocity.RotateAboutZ(-bank);
    position += velocity * Speed;
    position -= Centre;
    position.Z = 0.0f;
    position.AccurateNormalise();
    position *= CurrentRadius;
    position += Centre;
    position.Z = (Centre.Z - height) * 0.1f + height;
    velocity = position - oldPosition;
    velocity.Z = 0.0f;
    velocity.AccurateNormalise();
    velocity.RotateAboutZ(bank);
}