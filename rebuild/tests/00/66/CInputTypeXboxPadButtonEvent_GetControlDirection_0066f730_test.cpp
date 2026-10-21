// behaviour test for 0x0066F730 CInputTypeXboxPadButtonEvent::GetControlDirection
#include <stdio.h>
enum EXboxControllerButton { EXBOXCONTROLLERBUTTON_FORCE_DWORD = 0x7fffffff };
class C2DVector {                                       // Ego_r.pdb C2DVector, 8 bytes; non-POD (returned through a hidden pointer)
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent {};

class CInputTypeXboxPadButtonEvent {
public:
    virtual void _v00() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual void _v03() {}
    virtual C2DVector GetControlDirection(const CInputEvent& event) const;   // slot 4
    EXboxControllerButton Button; // +0x04
};

C2DVector CInputTypeXboxPadButtonEvent::GetControlDirection(const CInputEvent& event) const
{
    return C2DVector(0.0f, 0.0f);
}

int main()
{
    CInputTypeXboxPadButtonEvent o; const CInputTypeXboxPadButtonEvent* p = &o; CInputEvent e;
    C2DVector r = p->GetControlDirection(e);
    int ok = r.X == 0.0f && r.Y == 0.0f;
    printf(ok ? "0066f730_TEST PASS\n" : "0066f730_TEST FAIL\n");
    return ok ? 0 : 1;
}