// Behaviour test for CPlayer::GetFreeCamera (0x00487180): the C3DVector / CRightHandedSet / CCamera model and the body
// verbatim from final.cpp (the CPlayer base replaced by a plain struct holding the two cameras), the CCamera constructor
// and GetHorizontalFOV stubbed so the arguments are observable, the reciprocal-square-root table filled from the real
// formula. Checks the lerp at t = 0, 1 and 0.25, that forward/up come back unit length in the lerped direction, and that
// the FOV of the current free camera and a zero zoom reach the constructor.
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif

unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558 (filled in main)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline void Normalise()
    {
        float lengthSquared = X * X + (Y * Y + Z * Z);
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = 0.5f * (r * (3.0f - lengthSquared * (r * r)));
        X *= r;
        Y *= r;
        Z *= r;
    }
};
class CRightHandedSet { public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0c
};
class CCamera { public:                                        // 0x38 bytes
    C3DVector       WorldPosition;                      // +0x00
    CRightHandedSet RHSet;                              // +0x0c
    unsigned char   _body[0x38 - 0x24];
    CCamera() {}
    CCamera(const C3DVector& position, const C3DVector& forward, const C3DVector& up, float fov, float zoom);   // 0x00A0C130
    float GetHorizontalFOV() const;                     // 0x00A0BEB0
};

// ---- stubs ----
static int g_ctorCalls = 0;
static float g_fov = 0.0f;
static float g_zoom = 1.0f;
static int g_fovCalls = 0;
static const CCamera* g_fovCamera = 0;
CCamera::CCamera(const C3DVector& position, const C3DVector& forward, const C3DVector& up, float fov, float zoom)
{
    g_ctorCalls++;
    WorldPosition = position;
    RHSet.Forward = forward;
    RHSet.Up = up;
    g_fov = fov;
    g_zoom = zoom;
}
float CCamera::GetHorizontalFOV() const
{
    g_fovCalls++;
    g_fovCamera = this;
    return 1.25f;
}

struct CPlayer_Methods {
    CCamera FreeCamera;
    CCamera OldFreeCamera;
    const CCamera& FreeCam() const { return FreeCamera; }
    const CCamera& OldCam() const { return OldFreeCamera; }
    CCamera GetFreeCamera(float t) const;
};

// ---- body (verbatim from final.cpp) ----
CCamera CPlayer_Methods::GetFreeCamera(float t) const
{
    C3DVector position = (FreeCam().WorldPosition - OldCam().WorldPosition) * t + OldCam().WorldPosition;
    C3DVector forward = (FreeCam().RHSet.Forward - OldCam().RHSet.Forward) * t + OldCam().RHSet.Forward;
    forward.Normalise();
    C3DVector up = (FreeCam().RHSet.Up - OldCam().RHSet.Up) * t + OldCam().RHSet.Up;
    up.Normalise();
    return CCamera(position, forward, up, FreeCam().GetHorizontalFOV(), 0.0f);
}

// ---- test ----
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static bool Near(float a, float b, float eps) { return fabs((double)(a - b)) <= eps; }
static bool NearVec(const C3DVector& a, const C3DVector& b, float eps)
{
    return Near(a.X, b.X, eps) && Near(a.Y, b.Y, eps) && Near(a.Z, b.Z, eps);
}
static C3DVector Unit(const C3DVector& v)
{
    float len = (float)sqrt((double)(v.X * v.X + v.Y * v.Y + v.Z * v.Z));
    return C3DVector(v.X / len, v.Y / len, v.Z / len);
}
static C3DVector Lerp(const C3DVector& a, const C3DVector& b, float t)
{
    return C3DVector(a.X + (b.X - a.X) * t, a.Y + (b.Y - a.Y) * t, a.Z + (b.Z - a.Z) * t);
}

int main()
{
    // Table: index = (exponent low bit << 6) | top six mantissa bits; the entry is the top byte of the mantissa of
    // 1/sqrt(x) scaled so that the exponent (0x17c - e) >> 1 is right (x in [1,4) -> estimate = 0.5 * (1 + mantissa)).
    for (int i = 0; i < 128; ++i) {
        double m = 1.0 + ((i & 63) + 0.5) / 64.0;
        double x = m * ((i >> 6) ? 1.0 : 2.0);     // bit 6 set = odd exponent (x in [1,2)), clear = even (x in [2,4))
        double r = 1.0 / sqrt(x);
        double mant = 2.0 * r - 1.0;
        long entry = (long)(mant * 256.0);
        if (entry < 0) entry = 0;
        if (entry > 255) entry = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)entry;
    }

    CPlayer_Methods player;
    player.OldFreeCamera.WorldPosition = C3DVector(10.0f, 20.0f, 30.0f);
    player.OldFreeCamera.RHSet.Forward = C3DVector(1.0f, 0.0f, 0.0f);
    player.OldFreeCamera.RHSet.Up = C3DVector(0.0f, 1.0f, 0.0f);
    player.FreeCamera.WorldPosition = C3DVector(14.0f, 28.0f, 22.0f);
    player.FreeCamera.RHSet.Forward = C3DVector(0.0f, 0.0f, 3.0f);
    player.FreeCamera.RHSet.Up = C3DVector(0.0f, 2.0f, 2.0f);

    // t = 0: the old camera's position and (renormalised) axes.
    CCamera c0 = player.GetFreeCamera(0.0f);
    CHECK(g_ctorCalls == 1 && g_fovCalls == 1 && g_fovCamera == &player.FreeCamera);
    CHECK(Near(g_fov, 1.25f, 0.0f) && Near(g_zoom, 0.0f, 0.0f));
    CHECK(NearVec(c0.WorldPosition, C3DVector(10.0f, 20.0f, 30.0f), 0.0f));
    CHECK(NearVec(c0.RHSet.Forward, C3DVector(1.0f, 0.0f, 0.0f), 0.01f));
    CHECK(NearVec(c0.RHSet.Up, C3DVector(0.0f, 1.0f, 0.0f), 0.01f));

    // t = 1: the current camera's position, the axes renormalised to unit length.
    CCamera c1 = player.GetFreeCamera(1.0f);
    CHECK(g_ctorCalls == 2 && g_fovCalls == 2);
    CHECK(NearVec(c1.WorldPosition, C3DVector(14.0f, 28.0f, 22.0f), 0.0f));
    CHECK(NearVec(c1.RHSet.Forward, C3DVector(0.0f, 0.0f, 1.0f), 0.01f));
    CHECK(NearVec(c1.RHSet.Up, Unit(C3DVector(0.0f, 2.0f, 2.0f)), 0.01f));

    // t = 0.25: lerped position, lerped-then-normalised axes.
    CCamera c2 = player.GetFreeCamera(0.25f);
    CHECK(g_ctorCalls == 3 && g_fovCalls == 3);
    CHECK(NearVec(c2.WorldPosition, C3DVector(11.0f, 22.0f, 28.0f), 0.0001f));
    CHECK(NearVec(c2.RHSet.Forward, Unit(Lerp(C3DVector(1.0f, 0.0f, 0.0f), C3DVector(0.0f, 0.0f, 3.0f), 0.25f)), 0.01f));
    CHECK(NearVec(c2.RHSet.Up, Unit(Lerp(C3DVector(0.0f, 1.0f, 0.0f), C3DVector(0.0f, 2.0f, 2.0f), 0.25f)), 0.01f));
    float forwardLength = (float)sqrt((double)(c2.RHSet.Forward.X * c2.RHSet.Forward.X + c2.RHSet.Forward.Y * c2.RHSet.Forward.Y + c2.RHSet.Forward.Z * c2.RHSet.Forward.Z));
    CHECK(Near(forwardLength, 1.0f, 0.01f));

    printf("00487180_TEST %s\n", g_fail ? "FAIL" : "PASS");
    return g_fail ? 1 : 0;
}