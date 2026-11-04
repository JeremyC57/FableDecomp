// CTCFlourishTarget::ProcessActionMovementDelta(C3DVector* delta) -- re-express the action's movement delta in the frame
// Grade: DIFFER(ours v retail) -- DIFFER dl=16 (was 107): the three amounts as one C3DVector local(forward.Dot, side.Dot, up.Dot) (not float locals homed in the dead delta slot) gives retail's frame 0x3c and slots; shared CIntelligentPointer<CThing> target. Left: VC7.1 reassociates the dot sums: retail adds forward Y+Z then X and side X+Z then Y, ours Z+Y / X+(Z+Y), and multiplies forward.Z by the amount in the other operand order; every source order of Dot (and delta->Dot) gives the same canonical order. Best try/007e8a40_c (best try/007e8a40_c.cpp)
// Leaf: ?ProcessActionMovementDelta@CTCFlourishTarget@
// Flags: none
// facing the live flourish target on the ground plane (forward towards the target, sideways, world up), optionally
// forbidding backward and sideways movement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8A40, 482 bytes, `ret 4`, no frame pointer (speed TU).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CThing::GetPosition 0x004C73D0 (x2), C3DVector::Normalise
// 0x00A14440 (in place; label GetScaled).
// Data: 0x0129BA3C 1e-4f (squared), 0x0122DEDC 0.0f.
// Layout: retail CTCFlourishTarget ParentThing +0x04, target pointer +0x0C, NoBackwardMovement +0x14, NoSidewaysMovement
// +0x15 (names descriptive); CThing dead bit 0 of +0x91.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float MagSq() const { return X * X + Y * Y + Z * Z; }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    C3DVector Cross(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    void Normalise();                                       // 0x00A14440
};
extern const float g_MovementEpsilon;                       // 0x0129BA3C 1e-4f
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing : public CBaseObject {
public:
    unsigned char _pad08[0x91 - 0x08];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CTCFlourishTarget {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointer<CThing> Target;                     // +0x0C
    bool    NoBackwardMovement;                             // +0x14
    bool    NoSidewaysMovement;                             // +0x15
    void ProcessActionMovementDelta(C3DVector* delta);
};

void CTCFlourishTarget::ProcessActionMovementDelta(C3DVector* delta)
{
    if (!Target.Get() || Target.Get()->Dead)
        return;
    CThing* parent = ParentThing;
    CThing* target = Target.Get();
    const C3DVector& from = parent->GetPosition();
    const C3DVector& to = target->GetPosition();
    C3DVector forward(to.X - from.X, to.Y - from.Y, 0.0f);
    if (forward.MagSq() > g_MovementEpsilon * g_MovementEpsilon) {
        forward.Normalise();
        const C3DVector up(0.0f, 0.0f, 1.0f);
        C3DVector side = forward.Cross(up);
        C3DVector local(forward.Dot(*delta), side.Dot(*delta), up.Dot(*delta));
        if (NoBackwardMovement && local.X < 0.0f)
            local.X = 0.0f;
        if (NoSidewaysMovement)
            local.Y = 0.0f;
        *delta = forward * local.X + side * local.Y + up * local.Z;
    }
}
