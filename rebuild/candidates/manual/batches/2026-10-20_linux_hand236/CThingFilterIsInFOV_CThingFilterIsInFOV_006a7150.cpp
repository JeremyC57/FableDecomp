// CThingFilter_IsInFOV::CThingFilter_IsInFOV(const C3DVector& fromPos, const C3DVector& direction, float fov, float radius, bool angleTest3D) -- set up a view cone: cos of the half angle, normalised (optionally flattened) direction, and the eye pulled back so a cone of the given radius fits
// Leaf: ??0CThingFilter_IsInFOV
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A7150, 286 bytes (+2 int3), `ret 14h`, no calls.
// Globals: 0.5f 0x0122F59C, the double 6.2831854820251465 (2*pi as a float) 0x0124F2B8 fed to the fcos/fptan intrinsics,
//   0.25f 0x0123078C, 1e-4f 0x0129BA3C, 1.0f 0x0122DED8.
// Layout: PDB CThingFilter_IsInFOV { C3DVector FromPos +0; C3DVector Direction +0xC; float CosFOV +0x18; float FOV +0x1C;
//   bool AngleTest3D +0x20; } (no engine header).
// Shape: FOV is a fraction of a full turn: the half angle (kept in the dead fov slot) times (double)2pi gives CosFOV via
//   the cos intrinsic (FOV itself is a body assignment: retail stores AngleTest3D first); below a quarter turn the back-off distance radius / tan(angle) (the tan intrinsic, `fptan ;
//   fstp st0 ; fdivp`) lands in the dead angleTest3D slot, else 0. Direction is copied, flattened when the test is 2D,
//   normalised in place with the 1e-4 squared-length guard (1 / sqrt), and FromPos is moved back along it.
#include <math.h>
#pragma intrinsic(cos, tan, sqrt)

struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}   // the engine memberwise copy ctor
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise()
    {
        float lengthSquared = GetLengthSquared();
        if (lengthSquared > 1e-4f) {
            float r = 1.0f / sqrtf(lengthSquared);
            X *= r;
            Y *= r;
            Z *= r;
        }
    }
    C3DVector& operator-=(const C3DVector& v) { X -= v.X; Y -= v.Y; Z -= v.Z; return *this; }
};
inline C3DVector operator*(float s, const C3DVector& v) { return C3DVector(s * v.X, s * v.Y, s * v.Z); }
static const float TWO_PI = 6.2831855f;

struct CThingFilter_IsInFOV {
    C3DVector FromPos;                              // +0x00
    C3DVector Direction;                            // +0x0c
    float     CosFOV;                               // +0x18
    float     FOV;                                  // +0x1c
    bool      AngleTest3D;                          // +0x20
    CThingFilter_IsInFOV(const C3DVector& fromPos, const C3DVector& direction, float fov, float radius, bool angleTest3D);
};

CThingFilter_IsInFOV::CThingFilter_IsInFOV(const C3DVector& fromPos, const C3DVector& direction, float fov, float radius,
                                           bool angleTest3D)
    : FromPos(fromPos)
    , AngleTest3D(angleTest3D)
{
    FOV = fov;
    float halfFov = fov * 0.5f;
    CosFOV = (float)cos(halfFov * (double)TWO_PI);
    float backOff = 0.0f;
    if (halfFov < 0.25f)
        backOff = (float)(radius / tan(halfFov * (double)TWO_PI));
    Direction = direction;
    if (!AngleTest3D)
        Direction.Z = 0.0f;
    Direction.Normalise();
    FromPos -= backOff * Direction;
}
