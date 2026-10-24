// Behaviour test for 0x0066F8B0 CInputTypeMouseMovementEvent::GetControlDirection: mouse movement scaled by 1/64, Y
// flipped, clamped to unit length when longer than 1.
#include <stdio.h>
#include <math.h>
class C3DVector { public: float X, Y, Z; };
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    explicit C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class CInputEvent { public: C3DVector Movement; C3DVector GetMouseMovement() const { return Movement; } };
static const float g_MouseMovementScale = 0.015625f;
struct CInputTypeMouseMovementEvent_Methods { C2DVector GetControlDirection(const CInputEvent& event) const; };
C2DVector CInputTypeMouseMovementEvent_Methods::GetControlDirection(const CInputEvent& event) const
{
    C2DVector movement(event.GetMouseMovement());
    double x = g_MouseMovementScale * movement.X;
    double y = movement.Y;
    y *= g_MouseMovementScale;
    y = (float)y * -1.0f;
    double lengthSq = x * x;
    lengthSq += y * y;
    float length = (float)sqrt(lengthSq);
    if (length > 1.0f) {
        float scale = 1.0f / length;
        x *= scale;
        y *= scale;
    }
    return C2DVector((float)x, (float)y);
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    CInputTypeMouseMovementEvent_Methods t; CInputEvent e;
    e.Movement.X = 32.0f; e.Movement.Y = 16.0f; e.Movement.Z = 99.0f;
    C2DVector a = t.GetControlDirection(e);
    bool ok = Near(a.X, 0.5f) && Near(a.Y, -0.25f);
    e.Movement.X = 192.0f; e.Movement.Y = -256.0f;          // (3, 4) -> length 5 -> (0.6, 0.8)
    C2DVector b = t.GetControlDirection(e);
    ok = ok && Near(b.X, 0.6f) && Near(b.Y, 0.8f);
    e.Movement.X = 64.0f; e.Movement.Y = 0.0f;              // exactly 1: not rescaled
    C2DVector c = t.GetControlDirection(e);
    ok = ok && c.X == 1.0f && c.Y == 0.0f;
    if (!ok) { printf("0066f8b0_TEST FAIL\n"); return 1; }
    printf("0066f8b0_TEST PASS\n");
    return 0;
}
