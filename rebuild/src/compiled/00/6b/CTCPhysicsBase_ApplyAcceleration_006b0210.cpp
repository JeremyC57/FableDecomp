// CTCPhysicsBase::ApplyAcceleration(const C3DVector& acceleration) -- Velocity += acceleration, flag the parent thing as frame-updated and mark the body Moving
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B0210, 49 bytes.
// Callees: CThing::SetAsFrameUpdated @ 0x004C7C40 (on the parent thing at +0x04).
// Layout: engine/CTCPhysicsBase.h (Velocity +0x28, the parent thing +0x04); the PDB bool:1 Moving (bit 0 of +0x3c, the header's
//   opaque OldMoving[4] bytes) through a bit-field view.
// Shape: the inline C3DVector += (X member-first, Y/Z argument-first: the compiler's operand choice); `or byte ptr [esi+3Ch],1`.
// Leaf: ApplyAcceleration
#include "engine/CTCPhysicsBase.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};

struct SPhysicsBaseFlags {                  // CTCPhysicsBase +0x3c bool:1 group
    bool Moving : 1;                        // bit 0
    bool OldMoving : 1;                     // bit 1
};

class CThing { public:
    void SetAsFrameUpdated();               // 0x004C7C40
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    CThing* GetThing() { return reinterpret_cast<CThing*>(sub); }
    C3DVector& VelocityVector() { return *reinterpret_cast<C3DVector*>(&Velocity_X); }
    SPhysicsBaseFlags& Flags()  { return *reinterpret_cast<SPhysicsBaseFlags*>(OldMoving); }
    void ApplyAcceleration(const C3DVector& acceleration);
};

void CTCPhysicsBase_Methods::ApplyAcceleration(const C3DVector& acceleration)
{
    VelocityVector() += acceleration;
    GetThing()->SetAsFrameUpdated();
    Flags().Moving = true;
}