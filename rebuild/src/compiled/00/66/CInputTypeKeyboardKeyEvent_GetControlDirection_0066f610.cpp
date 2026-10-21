// CInputTypeKeyboardKeyEvent::GetControlDirection(const CInputEvent&) const -- the key's fixed control direction
// Leaf: GetControlDirection@CInputTypeKeyboardKeyEvent
// Retail: Fable.exe @ 0x0066F610, 18 bytes, vtable 0x012318A4 slot 4; label PDB-confirmed (labels_rtti_port). `ret 8`: hidden
// result pointer plus the event reference.
// Layout: engine/CInputTypeKeyboardKeyEvent.h / Ego_r.pdb CInputTypeKeyboardKeyEvent (0x10): vptr, KeyboardKey +0x04, ControlDirection +0x08 (C2DVector).
enum EInputKey { EINPUTKEY_FORCE_DWORD = 0x7fffffff };
class C2DVector {                                       // Ego_r.pdb C2DVector, 8 bytes; non-POD (returned through a hidden pointer)
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent;

class CInputTypeKeyboardKeyEvent {
public:
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual C2DVector GetControlDirection(const CInputEvent& event) const;   // slot 4
    EInputKey KeyboardKey;        // +0x04
    C2DVector ControlDirection;                         // +0x08
};

C2DVector CInputTypeKeyboardKeyEvent::GetControlDirection(const CInputEvent& event) const
{
    return ControlDirection;
}