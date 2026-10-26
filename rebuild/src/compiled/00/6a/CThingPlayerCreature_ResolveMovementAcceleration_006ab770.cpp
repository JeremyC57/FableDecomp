// CThingPlayerCreature::ResolveMovementAcceleration() -- clamp the movement acceleration to the current maximum movement speed and, when the player did not drive the movement this frame, damp it to 0.49
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB770, length 172 bytes, `ret`.
// Callees: CThingPlayerCreature::GetCurrentMaxMovementSpeed @ 0x006AB5D0 (engine_api.tsv, float).
// Float pool: 0x0125D134 = 0.49f.
// Layout: retail CThingPlayerCreature MovedByPlayer +0x178 bit 0 and MovementAcceleration +0x180 (Ego_r.pdb +0x1a0 /
//   +0x1a8: the retail creature chain sits 0x28 lower), so the class is modelled locally.
// Shape: the magnitude is the landed by-value Magnitude(x, y, z) helper (Z, Y, X loads, `fstp st(3)` cleanup); the clamp
//   scale `max / length` stays on the x87 stack for an inline C3DVector `*= float`; the damping is a second inline
//   `*= 0.49f` with the literal re-read per component.
#include <math.h>

inline float Magnitude(float x, float y, float z) { return (float)sqrt(x * x + y * y + z * z); }

class C3DVector { public:
    float X, Y, Z;
    float Length() const { return Magnitude(X, Y, Z); }
    void operator*=(float s) { X *= s; Y *= s; Z *= s; }
};

class CThingPlayerCreature {
public:
    unsigned char _pad00[0x178];
    unsigned char MovedByPlayer : 1;            // +0x178 bit 0
    unsigned char Jumping : 1;                  // bit 1
    unsigned char Pad1 : 1;                     // bit 2
    unsigned char _pad178 : 5;
    unsigned char _pad179[0x180 - 0x179];
    C3DVector MovementAcceleration;             // +0x180
    float GetCurrentMaxMovementSpeed();         // 0x006AB5D0
    void ResolveMovementAcceleration();
};

void CThingPlayerCreature::ResolveMovementAcceleration()
{
    float maxSpeed = GetCurrentMaxMovementSpeed();
    float length = MovementAcceleration.Length();
    if (length > maxSpeed)
        MovementAcceleration *= maxSpeed / length;
    if (!MovedByPlayer)
        MovementAcceleration *= 0.49f;
}