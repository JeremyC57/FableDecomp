// CInputTypeXboxPadLeftStickEvent::GetControlDirection(const CInputEvent& event) const -- stick deflection as a control direction, clamped to unit length
// Retail 0x0066F7D0, 106 bytes (Ego_r.pdb ?GetControlDirection@CInputTypeXboxPadLeftStickEvent@@UBE?AVC2DVector@@
// ABVCInputEvent@@@Z, virtual, `ret 8`: hidden C2DVector result + the event). The event's joystick position
// (CInputEvent::GetJoystickPosition @ 0x00A043A0, a C2DVector returned through a frame temporary) is turned
// into screen-up coordinates by negating Y (`fmul [-1.0f]`); when the deflection is longer than both the
// epsilon (the extern float 0x0129BA3C) and 1.0, it is scaled back to unit length. The length is homed in the
// dead `event` argument slot. Twin of the right-stick version 0x0066F840 (same body).
// Shape: the call result is the named direction itself (Y flipped in place, `*= -1.0f`), so both components stay
// on the x87 stack; squared X-first through a statement-wise GetLengthSquared; the unit scaling is written as a
// division of each component by the length (`X /= length; Y /= length;`): VC7.1 folds it into one reciprocal
// (`fld 1.0 ; fdiv length`) and multiplies X then Y (`fld st0 ; fmulp st2 ; fmulp st2`); a named `scale` local
// multiplies Y first whatever the statement order. Returned through the memberwise user copy constructor.
// Leaf: ?GetControlDirection@CInputTypeXboxPadLeftStickEvent

#include <math.h>
#pragma intrinsic(sqrt)

extern float g_epsilon_0129ba3c;                         // 0x0129BA3C (1e-4f)

class C2DVector
{
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
    float GetLengthSquared() const { float l = X * X; l += Y * Y; return l; }
    float GetLength() const { return (float)sqrt(GetLengthSquared()); }
    C2DVector& operator*=(float s) { X *= s; Y *= s; return *this; }
};

class CInputEvent
{
public:
    C2DVector GetJoystickPosition() const;               // 0x00A043A0
};

class CInputType
{
public:
    virtual C2DVector GetControlDirection(const CInputEvent& event) const = 0;
};

class CInputTypeXboxPadLeftStickEvent : public CInputType
{
public:
    virtual C2DVector GetControlDirection(const CInputEvent& event) const;
};

C2DVector CInputTypeXboxPadLeftStickEvent::GetControlDirection(const CInputEvent& event) const
{
    C2DVector direction = event.GetJoystickPosition();
    direction.Y *= -1.0f;
    float length = direction.GetLength();
    if (length > g_epsilon_0129ba3c && length > 1.0f)
    {
        direction.X /= length;
        direction.Y /= length;
    }
    return direction;
}
