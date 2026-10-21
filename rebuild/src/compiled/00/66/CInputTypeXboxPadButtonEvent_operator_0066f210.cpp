// CInputTypeXboxPadButtonEvent::operator==(const CInputTypeBase& other) -- same input type and the same Button
// Leaf: ??8CInputTypeXboxPadButtonEvent@@UBE_NABVCInputTypeBase@@@Z
// Retail: Fable.exe @ 0x0066F210, 52 bytes, `ret 4`; slot 2 of vtable 0x012317A8 (RTTI .?AVCInputTypeXboxPadButtonEvent@@); Ego_r.pdb
// ??8CInputTypeXboxPadButtonEvent@@UBE_NABVCInputTypeBase@@@Z. Both objects' slot-1 virtual (the input-type getter; this class
// returns 2 (0x00445620)) are compared -- other's called first -- then Button (+4) via `neg / sbb / inc`. The bytes are shared
// by the KeyboardKey, MouseButton and XboxPadButton instances (0x0066EDE0 / 0x0066EEA0 / 0x0066F210).
// Layout: CInputTypeXboxPadButtonEvent (Ego_r.pdb): vptr +0, Button +4 (8 bytes); engine/CInputTypeXboxPadButtonEvent.h models the
// vptr as a plain void*, so the class is declared locally with its CInputTypeBase virtuals (slot 0 the deleting
// destructor, slot 1 GetInputType, slot 2 operator==).
// Shape: /O2; the equal-type arm first (`jne` to the false tail) -- testing `!=` first lays the false arm inline.
enum EInputType {};
enum EXboxControllerButton {};
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual EInputType GetInputType() const = 0;
    virtual bool operator==(const CInputTypeBase& other) const = 0;
};
class CInputTypeXboxPadButtonEvent : public CInputTypeBase {
public:
    EXboxControllerButton Button;             // +0x04
    virtual EInputType GetInputType() const;
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeXboxPadButtonEvent::operator==(const CInputTypeBase& other) const
{
    if (GetInputType() == other.GetInputType())
        return Button == static_cast<const CInputTypeXboxPadButtonEvent&>(other).Button;
    return false;
}