// CInputTypeMouseButtonEvent::operator==(const CInputTypeBase& other) -- same input type and the same Button
// Leaf: ??8CInputTypeMouseButtonEvent@@UBE_NABVCInputTypeBase@@@Z
// Retail: Fable.exe @ 0x0066EEA0, 52 bytes, `ret 4`; slot 2 of vtable 0x012318C8 (RTTI .?AVCInputTypeMouseButtonEvent@@); Ego_r.pdb
// ??8CInputTypeMouseButtonEvent@@UBE_NABVCInputTypeBase@@@Z. Both objects' slot-1 virtual (the input-type getter; this class
// returns 1 (0x00447A70)) are compared -- other's called first -- then Button (+4) via `neg / sbb / inc`. The bytes are shared
// by the KeyboardKey, MouseButton and XboxPadButton instances (0x0066EDE0 / 0x0066EEA0 / 0x0066F210).
// Layout: CInputTypeMouseButtonEvent (Ego_r.pdb): vptr +0, Button +4, C2DVector ControlDirection +8 (0x10 bytes); engine/CInputTypeMouseButtonEvent.h models the
// vptr as a plain void*, so the class is declared locally with its CInputTypeBase virtuals (slot 0 the deleting
// destructor, slot 1 GetInputType, slot 2 operator==).
// Shape: /O2; the equal-type arm first (`jne` to the false tail) -- testing `!=` first lays the false arm inline.
enum EInputType {};
enum EMouseButtonControl {};
class C2DVector { public: float X; float Y; };
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual EInputType GetInputType() const = 0;
    virtual bool operator==(const CInputTypeBase& other) const = 0;
};
class CInputTypeMouseButtonEvent : public CInputTypeBase {
public:
    EMouseButtonControl Button;               // +0x04
    C2DVector ControlDirection;                          // +0x08
    virtual EInputType GetInputType() const;
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeMouseButtonEvent::operator==(const CInputTypeBase& other) const
{
    if (GetInputType() == other.GetInputType())
        return Button == static_cast<const CInputTypeMouseButtonEvent&>(other).Button;
    return false;
}