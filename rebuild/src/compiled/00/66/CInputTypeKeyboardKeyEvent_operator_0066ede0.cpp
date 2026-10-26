// CInputTypeKeyboardKeyEvent::operator==(const CInputTypeBase& other) -- same input type and the same KeyboardKey
// Leaf: ??8CInputTypeKeyboardKeyEvent@@UBE_NABVCInputTypeBase@@@Z
// Retail: Fable.exe @ 0x0066EDE0, 52 bytes, `ret 4`; slot 2 of vtable 0x012318A4 (RTTI .?AVCInputTypeKeyboardKeyEvent@@); Ego_r.pdb
// ??8CInputTypeKeyboardKeyEvent@@UBE_NABVCInputTypeBase@@@Z. Both objects' slot-1 virtual (the input-type getter; this class
// returns 0 (0x004479F0)) are compared -- other's called first -- then KeyboardKey (+4) via `neg / sbb / inc`. The bytes are shared
// by the KeyboardKey, MouseButton and XboxPadButton instances (0x0066EDE0 / 0x0066EEA0 / 0x0066F210).
// Layout: CInputTypeKeyboardKeyEvent (Ego_r.pdb): vptr +0, KeyboardKey +4, C2DVector ControlDirection +8 (0x10 bytes); engine/CInputTypeKeyboardKeyEvent.h models the
// vptr as a plain void*, so the class is declared locally with its CInputTypeBase virtuals (slot 0 the deleting
// destructor, slot 1 GetInputType, slot 2 operator==).
// Shape: /O2; the equal-type arm first (`jne` to the false tail) -- testing `!=` first lays the false arm inline.
enum EInputType {};
enum EInputKey {};
class C2DVector { public: float X; float Y; };
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual EInputType GetInputType() const = 0;
    virtual bool operator==(const CInputTypeBase& other) const = 0;
};
class CInputTypeKeyboardKeyEvent : public CInputTypeBase {
public:
    EInputKey KeyboardKey;                    // +0x04
    C2DVector ControlDirection;                          // +0x08
    virtual EInputType GetInputType() const;
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeKeyboardKeyEvent::operator==(const CInputTypeBase& other) const
{
    if (GetInputType() == other.GetInputType())
        return KeyboardKey == static_cast<const CInputTypeKeyboardKeyEvent&>(other).KeyboardKey;
    return false;
}