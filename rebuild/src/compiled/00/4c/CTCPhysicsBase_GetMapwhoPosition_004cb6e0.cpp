// CTCPhysicsBase::GetMapwhoPosition() -- the map-who (spatial index) position: Position shifted in X/Y by the virtual 2D offset (slot 28), Z unchanged
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CB6E0, 51 bytes (no direct callers: an out-of-line copy of the inline emitted in another TU; 0x006AFCB0 is the twin).
// Callees: CTCPhysicsBase virtual slot 28 (+0x70) = the 2D offset (C2DVector by hidden pointer; base 0x006B13D0 returns (0,0),
//   CTCPhysicsStandard 0x00724D30 returns OffsetVector3D.X/Y).
// Layout: engine/CTCPhysicsBase.h (Position +0x0c); the virtual is reached through a vtable-shaped view of `this`.
// Shape: the result built straight through the C3DVector value constructor inside a MEMBER OffsetBy2D(offset) of the
// position (this = Position, the 2D offset by reference: the offset operand is loaded first, `fld [eax+4] ; fadd [esi+10h]`);
// a free helper taking both by reference loads the position first. Arguments evaluated right to left (Z, then Y, then X).
// Leaf: GetMapwhoPosition
#include "engine/CTCPhysicsBase.h"

class C2DVector { public:
    float X, Y;
    C2DVector() { }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector OffsetBy2D(const C2DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z); }
};

struct CTCPhysicsBaseVirtuals {                    // vtable-shaped view of `this` (slot = offset / 4)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual C2DVector GetOffset2D();   // slot 28 (+0x70)
};

// The offset is applied by an inline MEMBER helper of the position taking the call result by reference: the fields are
// read through the returned pointer (eax), not from the frame temporary, and the offset operand is loaded first.

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    C3DVector GetMapwhoPosition();
};

C3DVector CTCPhysicsBase_Methods::GetMapwhoPosition()
{
    CTCPhysicsBaseVirtuals* self = reinterpret_cast<CTCPhysicsBaseVirtuals*>(this);
    return reinterpret_cast<const C3DVector*>(&Position_X)->OffsetBy2D(self->GetOffset2D());
}