// CInputTypeXboxPadRightStickEvent::GetControlDirection(const CInputEvent& event) const -- right stick as a direction, clamped to unit length
// Retail: Fable.exe @ 0x0066F840, 106 bytes, vtable 0x012317F0 slot 4, __thiscall `ret 8` (hidden C2DVector result,
// then the event). The stick position comes from the event's 2D accessor (0x00A043A0, hidden result in the frame);
// Y is flipped (`* -1.0f`), the length sqrt(X*X + Y*Y) is a named float local (homed in the dead `event` slot), and
// a vector longer than both the input dead zone (global 0x0129BA3C) and 1.0f is scaled by 1/length in registers.
// Shape: the components are double locals -- with float locals VC value-numbers x with position.X and squares it
// from memory (`fld [esp] ; fmul [esp]`); as doubles both stay on the FPU stack (`fld st(0) ; fmul st(1)`) and the
// scale is duplicated with `fld st(0)`, as retail.
// The left-stick twin 0x0066F7D0 has the identical body.
#include <math.h>
#include "engine/CInputTypeXboxPadRightStickEvent.h"
class C2DVector {
public:
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CInputEvent {
public:
public:
    C2DVector GetJoystickPosition() const;   // 0x00A043A0
};
extern const float g_InputDeadZone;             // 0x0129BA3C (0.0001f)
struct CInputTypeXboxPadRightStickEvent_Methods : CInputTypeXboxPadRightStickEvent {
    C2DVector GetControlDirection(const CInputEvent& event) const;
};
C2DVector CInputTypeXboxPadRightStickEvent_Methods::GetControlDirection(const CInputEvent& event) const
{
    C2DVector position = event.GetJoystickPosition();
    double y = position.Y * -1.0f;
    double x = position.X;
    float length = (float)sqrt(x * x + y * y);
    if (length > g_InputDeadZone && length > 1.0f) {
        float scale = 1.0f / length;
        x *= scale;
        y *= scale;
    }
    return C2DVector((float)x, (float)y);
}
