// CTCPhysicsBase::GetMapwhoPosition() -- the map-who (spatial index) position: Position shifted in X/Y by the virtual 2D offset (slot 28), Z unchanged
// Grade: DIFFER(ours v retail) -- 53v53: the two fadd pairs take the other operand order (lane: fld [esi+0Ch] ; fadd [eax]; retail: fld [eax] ; fadd [esi+0Ch]); a frame-local offset gets retail's order but reads the frame, not eax
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFCB0, 53 bytes (non-virtual; nine callers).
// Callees: CTCPhysicsBase virtual slot 28 (+0x70) = the 2D offset (C2DVector by hidden pointer; base 0x006B13D0 returns (0,0),
//   CTCPhysicsStandard 0x00724D30 returns OffsetVector3D.X/Y).
// Layout: engine/CTCPhysicsBase.h (Position +0x0c); the virtual is reached through a vtable-shaped view of `this`.
// Shape: the result built field by field in a NAMED local and returned through the memberwise copy (X, Y, Z order, `fxch st(2)` then three fstp).
// Leaf: GetMapwhoPosition
#include "engine/CTCPhysicsBase.h"

struct C2DVector {
    float X, Y;
    C2DVector() { }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct CTCPhysicsBaseVirtuals {                    // vtable-shaped view of `this` (slot = offset / 4)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual C2DVector GetOffset2D();   // slot 28 (+0x70)
};

// The offset is applied by an inline helper taking the call result by reference: the fields are read through the
// returned pointer (eax), not from the frame temporary.
inline C3DVector OffsetBy2D(const C3DVector& position, const C2DVector& offset)
{
    C3DVector result;
    result.X = offset.X + position.X;
    result.Y = offset.Y + position.Y;
    result.Z = position.Z;
    return result;
}

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    C3DVector GetMapwhoPosition();
};

C3DVector CTCPhysicsBase_Methods::GetMapwhoPosition()
{
    CTCPhysicsBaseVirtuals* self = reinterpret_cast<CTCPhysicsBaseVirtuals*>(this);
    return OffsetBy2D(*reinterpret_cast<const C3DVector*>(&Position_X), self->GetOffset2D());
}
