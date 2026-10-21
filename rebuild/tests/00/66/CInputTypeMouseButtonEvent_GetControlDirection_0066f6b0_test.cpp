// behaviour test for 0x0066F6B0 CInputTypeMouseButtonEvent::GetControlDirection
#include <stdio.h>
enum EMouseButtonControl { EMOUSEBUTTONCONTROL_FORCE_DWORD = 0x7fffffff };
class C2DVector {                                       // Ego_r.pdb C2DVector, 8 bytes; non-POD (returned through a hidden pointer)
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent {};

class CInputTypeMouseButtonEvent {
public:
    virtual void _v00() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual void _v03() {}
    virtual C2DVector GetControlDirection(const CInputEvent& event) const;   // slot 4
    EMouseButtonControl Button;   // +0x04
    C2DVector ControlDirection;                         // +0x08
};

C2DVector CInputTypeMouseButtonEvent::GetControlDirection(const CInputEvent& event) const
{
    return ControlDirection;
}

int main()
{
    CInputTypeMouseButtonEvent o; const CInputTypeMouseButtonEvent* p = &o; CInputEvent e;
    o.ControlDirection = C2DVector(0.5f, -1.0f);
    C2DVector r = p->GetControlDirection(e);
    int ok = r.X == 0.5f && r.Y == -1.0f;
    printf(ok ? "0066f6b0_TEST PASS\n" : "0066f6b0_TEST FAIL\n");
    return ok ? 0 : 1;
}