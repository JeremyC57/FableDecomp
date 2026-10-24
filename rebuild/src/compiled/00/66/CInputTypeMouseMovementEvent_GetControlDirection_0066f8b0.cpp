// CInputTypeMouseMovementEvent::GetControlDirection(const CInputEvent& event) const -- mouse movement as a direction, clamped to unit length
// Leaf: GetControlDirection
// Retail: Fable.exe @ 0x0066F8B0, 120 bytes, vtable 0x01231814 slot 4, __thiscall `ret 8` (hidden C2DVector result,
// then the event). The movement comes from CInputEvent's C3DVector accessor (0x00A043D0, copies +0x0C..+0x14;
// manifest label CTCPhysicsRigidBody::GetFacingVector is propagated), X and Y are scaled by 1/64 (0.015625f) and Y
// is flipped; a vector longer than 1.0f is scaled by 1/length. Double component locals keep the values on the FPU
// stack, as in the right-stick twin 0x0066F840.
// Shape: Y is built in sequenced double statements (load, *= scale, then a float-typed * -1.0f so the constant stays a
// dword) and the squared length as `lengthSq = x*x; lengthSq += y*y;`: the one-expression forms let VC7.1 hoist the
// scale load above the Y self-copy store and square Y first (120v120 x87 order).
#include <math.h>
#include "engine/CInputTypeMouseMovementEvent.h"
class C3DVector {
public:
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
};
class C2DVector {
public:
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    explicit C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class CInputEvent {
public:
    C3DVector GetMouseMovement() const;         // 0x00A043D0
};
extern const float g_MouseMovementScale;        // 0x0125B24C (0.015625f = 1/64)
struct CInputTypeMouseMovementEvent_Methods : CInputTypeMouseMovementEvent {
    C2DVector GetControlDirection(const CInputEvent& event) const;
};
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
