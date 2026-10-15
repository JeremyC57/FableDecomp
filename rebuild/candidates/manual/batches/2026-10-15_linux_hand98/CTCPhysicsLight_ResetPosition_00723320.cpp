// CTCPhysicsLight::ResetPosition(const C3DVector& pos) -- SetPosition(pos) through the vtable, then OldPosition = Position (no interpolation)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723320, 41 bytes, CTCPhysicsLight vtable 0x01265DFC slot 31.
// Callees: virtual SetPosition (slot 30, +0x78; CTCPhysicsLight 0x007233B0).
// Layout: engine/CTCPhysicsBase.h (Position +0x0c, OldPosition +0x18) viewed as C3DVectors; CTCPhysicsLight adds only
//   AffectedByGravity at +0x40.
// Shape: the whole-vector copy loads all three words (eax/ecx/edx) before `add esi,18h` and the three stores.
// Leaf: ResetPosition
#include "engine/CTCPhysicsBase.h"

struct C3DVector {
    float X, Y, Z;
};

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
    C3DVector& PositionVector()    { return *reinterpret_cast<C3DVector*>(&Position_X); }
    C3DVector& OldPositionVector() { return *reinterpret_cast<C3DVector*>(&OldPosition_X); }
    void ResetPosition(const C3DVector& pos);
};

void CTCPhysicsLight::ResetPosition(const C3DVector& pos)
{
    reinterpret_cast<CTCPhysicsLightVirtuals*>(this)->SetPosition(pos);
    OldPositionVector() = PositionVector();
}
