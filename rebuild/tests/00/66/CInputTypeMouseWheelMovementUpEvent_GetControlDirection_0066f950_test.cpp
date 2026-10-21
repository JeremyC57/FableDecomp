// behaviour test for 0x0066F950
#include <stdio.h>
class C2DVector {                                       // Ego_r.pdb C2DVector, 8 bytes; non-POD (returned through a hidden pointer)
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent {
public:
    float wheel;
    float GetMouseWheelMovement() const { return wheel; }
};
class CInputTypeMouseWheelMovementUpEvent {
public:
    virtual void _v00() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual void _v03() {}
    virtual C2DVector GetControlDirection(const CInputEvent& event) const;   // slot 4
};

C2DVector CInputTypeMouseWheelMovementUpEvent::GetControlDirection(const CInputEvent& event) const
{
    return C2DVector(0.0f, event.GetMouseWheelMovement());
}

int main()
{
    CInputTypeMouseWheelMovementUpEvent t; CInputEvent e; e.wheel = -2.5f;
    C2DVector d = t.GetControlDirection(e);
    int ok = d.X == 0.0f && d.Y == -2.5f;
    printf(ok ? "0066f950_TEST PASS\n" : "0066f950_TEST FAIL\n");
    return ok ? 0 : 1;
}