// CTCPhysicsLight::Move() -- integrate one step: Position += Velocity, gravity pulls Velocity.Z down when the body is affected by it, then SetPosition(Position)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723360, 68 bytes, CTCPhysicsLight vtable 0x01265DFC slot 87.
// Callees: virtual slot 48 (+0xc0; CTCPhysicsLight 0x00723440 returns the AffectedByGravity bit), virtual SetPosition (slot 30, +0x78).
//   0x01265DF0 = the per-step gravity decrement (float).
// Layout: engine/CTCPhysicsBase.h (Position +0x0c, Velocity +0x28) viewed as C3DVectors; AffectedByGravity bool:1 at +0x40.
// Shape: the Position reference lands in edi (`lea edi,[esi+0Ch]`) and is passed on to SetPosition.
// Leaf: Move
#include "engine/CTCPhysicsBase.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};

extern float g_fGravityStep;                // 0x01265DF0

struct CTCPhysicsLightVirtuals {                    // vtable-shaped view of `this` (slot = offset / 4)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void SetPosition(const C3DVector& pos);   // slot 30 (+0x78)
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual bool IsAffectedByGravity();   // slot 48 (+0xc0)
};

struct CTCPhysicsLight : CTCPhysicsBase {
    bool AffectedByGravity : 1;             // +0x40 bit 0
    C3DVector& PositionVector() { return *reinterpret_cast<C3DVector*>(&Position_X); }
    C3DVector& VelocityVector() { return *reinterpret_cast<C3DVector*>(&Velocity_X); }
    void Move();
};

void CTCPhysicsLight::Move()
{
    CTCPhysicsLightVirtuals* self = reinterpret_cast<CTCPhysicsLightVirtuals*>(this);
    PositionVector() += VelocityVector();
    if (self->IsAffectedByGravity())
        Velocity_Z -= g_fGravityStep;
    self->SetPosition(PositionVector());
}
