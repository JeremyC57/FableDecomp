// CTCFlourishTarget::ProcessActionMovementDelta(C3DVector& delta) const -- re-express the action's movement delta in the frame
// Grade: DIFFER(482v482) -- x87 operand order only (18 masked diff lines; frame now retail's 0x3C): forward dot Z,Y,X in ours v Y,Z,X in retail, side dot Z(st),Y,X v X,Z(st),Y, and the third product of forward * local.X loads forward.Z first (retail local.X first). Wave 25 (agent647): the projected delta as one C3DVector local fixed the frame (128 -> 18 lines); member-wise local, delta.Dot(v), f * X operator*, named forward/side floats feeding the local: all identical.
// facing the live flourish target on the ground plane (forward towards the target, sideways, world up), optionally
// forbidding backward and sideways movement
// Leaf: ?ProcessActionMovementDelta@CTCFlourishTarget@@QBEXAAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8A40, 482 bytes, `ret 4`, no frame pointer (speed TU).
//   Ego_r.pdb ?ProcessActionMovementDelta@CTCFlourishTarget@@QBEXAAVC3DVector@@@Z (const, by reference).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CThing::GetPos 0x004C73D0 (x2),
//   C3DVector::AccurateNormalise 0x00A14440 (in place, returns *this).
// Data: 0x0129BA3C 1e-4f (squared), 0x0122DEDC 0.0f (folded literal).
// Layout: retail CTCFlourishTarget ParentThing +0x04 (CTCBase), target intelligent pointer +0x0C, NoBackwardMovement +0x14,
//   NoSidewaysMovement +0x15 (names descriptive); CThing dead bit 0 of +0x91.
// Shape: the three projections are one C3DVector local (retail frame 0x3C: forward 0..0xB, the projected delta 0xC..0x17
//   with its Y kept on the x87 stack, side 0x18.., the scaled sums above), not three float locals (frame 0x34, the forward
//   amount homed in the dead argument slot).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float MagSq() const { return X * X + Y * Y + Z * Z; }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    C3DVector Cross(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector& AccurateNormalise();                         // 0x00A14440
};
extern const float g_MovementEpsilon;                       // 0x0129BA3C 1e-4f
class CBaseObject {
public:
    virtual void v00();
};
class CThing : public CBaseObject {
public:
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    const C3DVector& GetPos() const;                        // 0x004C73D0
};
class CBaseIntelligentPointer {
public:
    void* PData;
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return static_cast<T*>(GetPItem()); }
    T* Get() const { return static_cast<T*>(GetPItem()); }
};
class CTCFlourishTarget {
public:
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointer<CThing> Target;                     // +0x0C
    unsigned char _pad10[0x14 - 0x10];
    bool    NoBackwardMovement;                             // +0x14
    bool    NoSidewaysMovement;                             // +0x15
    void ProcessActionMovementDelta(C3DVector& delta) const;
};

void CTCFlourishTarget::ProcessActionMovementDelta(C3DVector& delta) const
{
    if (!Target.Get() || Target->Dead)
        return;
    CThing* parent = ParentThing;
    CThing* target = Target.Get();
    const C3DVector& from = parent->GetPos();
    const C3DVector& to = target->GetPos();
    C3DVector forward(to.X - from.X, to.Y - from.Y, 0.0f);
    if (forward.MagSq() > g_MovementEpsilon * g_MovementEpsilon) {
        forward.AccurateNormalise();
        const C3DVector up(0.0f, 0.0f, 1.0f);
        C3DVector side = forward.Cross(up);
        C3DVector local(forward.Dot(delta), side.Dot(delta), up.Dot(delta));
        if (NoBackwardMovement && local.X < 0.0f)
            local.X = 0.0f;
        if (NoSidewaysMovement)
            local.Y = 0.0f;
        delta = forward * local.X + side * local.Y + up * local.Z;
    }
}
