// Behaviour test for 0x0066F7D0 CInputTypeXboxPadLeftStickEvent::GetControlDirection: the real body with a
// same-TU joystick-position stub and the epsilon global.
#include <stdio.h>
#include <math.h>
float g_epsilon_0129ba3c = 0.0001f;
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
    C2DVector pos;
    C2DVector GetJoystickPosition() const { return pos; }
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
static bool Near(float a, float b) { return fabs(a - b) < 0.0001f; }
int main()
{
    CInputTypeXboxPadLeftStickEvent stick;
    const CInputType& type = stick;
    CInputEvent e;
    e.pos = C2DVector(0.3f, 0.4f);          // inside the unit circle: only Y flips
    C2DVector d = type.GetControlDirection(e);
    if (!Near(d.X, 0.3f) || !Near(d.Y, -0.4f)) { printf("0066f7d0_TEST FAIL 1\n"); return 1; }
    e.pos = C2DVector(3.0f, 4.0f);          // length 5: normalised
    d = type.GetControlDirection(e);
    if (!Near(d.X, 0.6f) || !Near(d.Y, -0.8f)) { printf("0066f7d0_TEST FAIL 2\n"); return 1; }
    e.pos = C2DVector(0.0f, 0.0f);          // below the epsilon: untouched
    d = type.GetControlDirection(e);
    if (!Near(d.X, 0.0f) || !Near(d.Y, 0.0f)) { printf("0066f7d0_TEST FAIL 3\n"); return 1; }
    printf("0066f7d0_TEST PASS\n");
    return 0;
}
