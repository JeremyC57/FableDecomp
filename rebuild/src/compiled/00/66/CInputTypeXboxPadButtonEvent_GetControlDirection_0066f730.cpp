// CInputTypeXboxPadButtonEvent::GetControlDirection(const CInputEvent&) const -- a pad button has no direction: the zero vector
// Leaf: GetControlDirection@CInputTypeXboxPadButtonEvent
// Retail: Fable.exe @ 0x0066F730, 20 bytes, vtable 0x012317A8 slot 4; label PDB-confirmed (labels_rtti_port). `ret 8`: hidden
// result pointer plus the event reference.
// Layout: engine/CInputTypeXboxPadButtonEvent.h / Ego_r.pdb CInputTypeXboxPadButtonEvent (8 bytes): vptr, Button +0x04.
enum EXboxControllerButton { EXBOXCONTROLLERBUTTON_FORCE_DWORD = 0x7fffffff };
class C2DVector {                                       // Ego_r.pdb C2DVector, 8 bytes; non-POD (returned through a hidden pointer)
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent;

class CInputTypeXboxPadButtonEvent {
public:
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual C2DVector GetControlDirection(const CInputEvent& event) const;   // slot 4
    EXboxControllerButton Button; // +0x04
};

C2DVector CInputTypeXboxPadButtonEvent::GetControlDirection(const CInputEvent& event) const
{
    return C2DVector(0.0f, 0.0f);
}