// CInputTypeMouseWheelMovementUpEvent::GetControlDirection(const CInputEvent&) const -- the wheel movement as a vertical control direction (0, movement)
// Leaf: GetControlDirection@CInputTypeMouseWheelMovementUpEvent
// Retail: Fable.exe @ 0x0066F950, 25 bytes, `ret 8` (hidden C2DVector result plus the event); referenced from the vtable slot at
// 0x0123186C (?GetControlDirection@CInputTypeMouseWheelMovementUpEvent@@UBE?AVC2DVector@@ABVCInputEvent@@@Z, slot 4 as for the other CInputType* rows).
// Callee 0x00A03BB0 (`fld [ecx+14h] ; ret`, identical-code-folded with the catalog's CMap::DrawGetSeaLevel) is
// ?GetMouseWheelMovement@CInputEvent@@QBEMXZ, the only float CInputEvent getter besides the joystick pressure.
class C2DVector {                                       // Ego_r.pdb C2DVector, 8 bytes; non-POD (returned through a hidden pointer)
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent {
public:
    float GetMouseWheelMovement() const;                 // 0x00A03BB0
};
class CInputTypeMouseWheelMovementUpEvent {
public:
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual C2DVector GetControlDirection(const CInputEvent& event) const;   // slot 4
};

C2DVector CInputTypeMouseWheelMovementUpEvent::GetControlDirection(const CInputEvent& event) const
{
    return C2DVector(0.0f, event.GetMouseWheelMovement());
}