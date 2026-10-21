// Grade: DIFFER(482v482) -- frame 0x34 v 0x3c: ours homes the forward amount in the dead delta argument slot and packs the vector temporaries 8 bytes tighter; the side dot sums x,z,y in retail v z,y,x in ours. Fixed: named parent/target locals give retail's call order (478 -> 482). Tried: delta->Dot forms (no change).
// CTCFlourishTarget::ProcessActionMovementDelta(C3DVector* delta) -- re-express the action's movement delta in the frame
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
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void* m_pItem;
    CThing* GetPItem() const;                               // 0x00A01B50
};
struct CTCFlourishTarget {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CBaseIntelligentPointer Target;                         // +0x0C
    unsigned char _pad10[0x14 - 0x10];
    bool    NoBackwardMovement;                             // +0x14
    bool    NoSidewaysMovement;                             // +0x15
    void ProcessActionMovementDelta(C3DVector* delta);
};

void CTCFlourishTarget::ProcessActionMovementDelta(C3DVector* delta)
{
    if (!Target.GetPItem() || Target.GetPItem()->Dead)
        return;
    CThing* parent = ParentThing;
    CThing* target = Target.GetPItem();
    const C3DVector& from = parent->GetPosition();
    const C3DVector& to = target->GetPosition();
    C3DVector forward(to.X - from.X, to.Y - from.Y, 0.0f);
    if (forward.MagSq() > g_MovementEpsilon * g_MovementEpsilon) {
        forward.Normalise();
        const C3DVector up(0.0f, 0.0f, 1.0f);
        C3DVector side = forward.Cross(up);
        float forwardAmount = forward.Dot(*delta);
        float sideAmount = side.Dot(*delta);
        float upAmount = up.Dot(*delta);
        if (NoBackwardMovement && forwardAmount < 0.0f)
            forwardAmount = 0.0f;
        if (NoSidewaysMovement)
            sideAmount = 0.0f;
        *delta = forward * forwardAmount + side * sideAmount + up * upAmount;
    }
}
