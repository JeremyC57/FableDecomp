// CInputTypeMouseButtonEvent::GetControlDirection(const CInputEvent&) const -- the mouse button's fixed control direction
// Leaf: GetControlDirection@CInputTypeMouseButtonEvent
// Retail: Fable.exe @ 0x0066F6B0, 18 bytes, vtable 0x012318C8 slot 4; label PDB-confirmed (labels_rtti_port). `ret 8`: hidden
// result pointer plus the event reference.
// Layout: engine/CInputTypeMouseButtonEvent.h / Ego_r.pdb CInputTypeMouseButtonEvent (0x10): vptr, Button +0x04, ControlDirection +0x08 (C2DVector).
enum EMouseButtonControl { EMOUSEBUTTONCONTROL_FORCE_DWORD = 0x7fffffff };
class C2DVector {                                       // Ego_r.pdb C2DVector, 8 bytes; non-POD (returned through a hidden pointer)
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent;

class CInputTypeMouseButtonEvent {
public:
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual C2DVector GetControlDirection(const CInputEvent& event) const;   // slot 4
    EMouseButtonControl Button;   // +0x04
    C2DVector ControlDirection;                         // +0x08
};

C2DVector CInputTypeMouseButtonEvent::GetControlDirection(const CInputEvent& event) const
{
    return ControlDirection;
}