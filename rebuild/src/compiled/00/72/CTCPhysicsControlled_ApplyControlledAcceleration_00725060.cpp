// CTCPhysicsControlled::ApplyControlledAcceleration(const C3DVector& acceleration, float maxSpeed) -- below the speed limit, apply 5% of the acceleration's part off its own projection; at or above it, turn the velocity towards the acceleration by at most 0.007 of a turn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00725060, length 494 bytes (+ int3 padding), esp frame, `ret 8`.
// Callees: CTCPhysicsBase::ApplyAcceleration(const C3DVector&) @ 0x006B0210, _CIfmod @ 0x00BFEB28 (the intrinsic fmod of
// the inline WrapAngle), AngleDifference @ 0x00A441E0 (`__stdcall`), C3DVector::Rotate(const C3DVector& axis, float) @ 0x00A13C60.
// Constants: 0.05f @ 0x0125A15C, 1/(2pi) @ 0x01237F08, 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, 1.0 @ 0x01237F00,
// -0.007f @ 0x012662F8, 0.007f @ 0x0125A164 (the clamp stores them as immediates).
// Shape: C3DVector has a memberwise USER copy ctor, so the named copy of the acceleration is scalar-replaced (X/Z as dword
// copies, Y kept on the x87 stack across the speed test); Velocity is reached through a named reference local (otherwise
// `this` moves to edi and the acceleration pointer to ecx); the dot product is `X*X' + (Y*Y' + Z*Z')` (retail emits the Y
// and Z terms first); the turn is WrapAngle(atan2(X, Y) / 2pi) for both vectors, AngleDifference(velocity, acceleration)
// clamped by an if/else-if to +-0.007, then a rotation about +Z.
// Layout: Ego_r.pdb CTCPhysicsBase Velocity +0x28 (Position +0x0c, OldPosition +0x18).
#include <math.h>
#pragma intrinsic(atan2, fmod, sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Rotate(const C3DVector& axis, float angle);    // 0x00A13C60
};
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + (a.Y * b.Y + a.Z * b.Z); }
float __stdcall AngleDifference(float a, float b);      // 0x00A441E0

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

struct CTCPhysicsBase {
    void*         __vftable;                            // +0x00
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                             // +0x0c
    C3DVector     OldPosition;                          // +0x18
    long          OldPositionLastFrameSet;              // +0x24
    C3DVector     Velocity;                             // +0x28
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CTCPhysicsControlled : CTCPhysicsBase {
    void ApplyControlledAcceleration(const C3DVector& acceleration, float maxSpeed);
};

void CTCPhysicsControlled::ApplyControlledAcceleration(const C3DVector& acceleration, float maxSpeed)
{
    C3DVector direction = acceleration;
    C3DVector& velocity = Velocity;
    if (velocity.GetLength() < maxSpeed) {
        C3DVector sideways = acceleration - direction * DotProduct(direction, velocity);
        ApplyAcceleration(sideways * 0.05f);
    } else {
        float accelerationAngle = WrapAngle((float)atan2(acceleration.X, acceleration.Y) * 0.15915494f);
        float velocityAngle = WrapAngle((float)atan2(velocity.X, velocity.Y) * 0.15915494f);
        float turn = AngleDifference(velocityAngle, accelerationAngle);
        if (turn < -0.007f)
            turn = -0.007f;
        else if (turn > 0.007f)
            turn = 0.007f;
        velocity.Rotate(C3DVector(0.0f, 0.0f, 1.0f), turn);
    }
}