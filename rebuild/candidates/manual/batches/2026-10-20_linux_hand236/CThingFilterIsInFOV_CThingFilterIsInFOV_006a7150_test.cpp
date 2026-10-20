#include <cstdio>
#include <math.h>

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
static bool near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    C3DVector from(10.0f, 10.0f, 10.0f), dir(3.0f, 0.0f, 4.0f);
    CThingFilter_IsInFOV a(from, dir, 0.25f, 2.0f, true);     // half 1/8 turn = 45 deg: cos .7071, tan 1 -> back off 2
    bool ok = near(a.CosFOV, 0.70710678f) && a.FOV == 0.25f && a.AngleTest3D
        && near(a.Direction.X, 0.6f) && near(a.Direction.Y, 0.0f) && near(a.Direction.Z, 0.8f)
        && near(a.FromPos.X, 8.8f) && near(a.FromPos.Y, 10.0f) && near(a.FromPos.Z, 8.4f);
    C3DVector dir2(3.0f, 4.0f, 5.0f);
    CThingFilter_IsInFOV b(from, dir2, 0.6f, 2.0f, false);    // half 0.3 turn >= 0.25: no back off; 2D: Z flattened
    ok = ok && !b.AngleTest3D && near(b.Direction.X, 0.6f) && near(b.Direction.Y, 0.8f) && b.Direction.Z == 0.0f
        && b.FromPos.X == 10.0f && b.FromPos.Y == 10.0f && b.FromPos.Z == 10.0f && near(b.CosFOV, (float)cos(0.3 * 6.2831855));
    C3DVector tiny(0.001f, 0.0f, 0.0f);
    CThingFilter_IsInFOV c(from, tiny, 0.6f, 2.0f, true);      // squared length under 1e-4: left unnormalised
    ok = ok && c.Direction.X == 0.001f;
    if (!ok) { std::printf("006a7150_TEST FAIL\n"); return 1; }
    std::printf("006a7150_TEST PASS\n");
    return 0;
}
