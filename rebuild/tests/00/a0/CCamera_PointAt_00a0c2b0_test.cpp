#include <stdio.h>
#include <string.h>
#include <math.h>
// CCamera::PointAt(const C3DVector& target) -- aims the camera: Forward = target - WorldPosition (Z flattened to 0 when ViewVecZLocked), then Up = world Z minus its projection on Forward, both normalised
// Leaf: PointAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0C2B0, 208 bytes incl. padding (ret 4).
// Callees: C3DVector::Normalise @ 0x00A14440 (manifest "GetScaled" is propagated), four times through the inline
//          CRightHandedSet::Normalise (Up then Forward). Float pool: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout (Ego_r.pdb CCamera, 56 bytes): WorldPosition +0, RHSet +0xC {Up +0xC, Forward +0x18}, HeightLocked +0x24,
//          ViewVecZLocked +0x25.
// Shape: the flag is an if/else whose arms both assign through a Forward reference taken first (lea esi before the
//          x87 work): locked builds C3DVector(dx, dy, 0), unlocked uses target - WorldPosition; VC7.1 merges the common X/Y
//          head above the branch. C3DVector has a user ctor + user copy ctor and no operator=, so each assignment builds a frame temporary and
//          copies it through a pointer register; Dot with (0,0,1) keeps the 0.0f products (x87, no fast-math).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Normalise();                           // 0x00A14440
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector operator*(const C3DVector& a, float s) { return C3DVector(a.X * s, a.Y * s, a.Z * s); }
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CRightHandedSet { public:
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0C
    void Normalise() { Up.Normalise(); Forward.Normalise(); }
};

class CCamera { public:
    C3DVector       WorldPosition;              // +0x00
    CRightHandedSet RHSet;                      // +0x0C
    bool            HeightLocked;               // +0x24
    bool            ViewVecZLocked;             // +0x25
    unsigned long   FOVFlags;                   // +0x28
    float           HorizontalFOV;              // +0x2C
    float           VerticalFOV;                // +0x30
    float           Zoom;                       // +0x34
    void PointAt(const C3DVector& target);
};

void CCamera::PointAt(const C3DVector& target)
{
    C3DVector& forward = RHSet.Forward;
    if (ViewVecZLocked)
        forward = C3DVector(target.X - WorldPosition.X, target.Y - WorldPosition.Y, 0.0f);
    else
        forward = target - WorldPosition;
    RHSet.Normalise();
    C3DVector worldUp(0.0f, 0.0f, 1.0f);
    RHSet.Up = worldUp - RHSet.Forward * Dot(RHSet.Forward, worldUp);
    RHSet.Normalise();
}

// ---- test ----
static int g_norm;
void C3DVector::Normalise() { g_norm++; float l = (float)sqrt(X * X + Y * Y + Z * Z); if (l > 0.0f) { X /= l; Y /= l; Z /= l; } }
static int fails;
static void check(int ok, const char* what) { if (!ok) { char b[120]; _snprintf(b, sizeof b, "FAIL %s", what); puts(b); fails++; } }
static int near(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }
int main()
{
    CCamera cam; memset(&cam, 0, sizeof cam);
    cam.WorldPosition = C3DVector(1.0f, 2.0f, 3.0f);
    cam.ViewVecZLocked = false;
    cam.PointAt(C3DVector(4.0f, 6.0f, 3.0f));        // direction (3,4,0) -> (0.6,0.8,0)
    check(g_norm == 4, "four normalises");
    check(near(cam.RHSet.Forward.X, 0.6f) && near(cam.RHSet.Forward.Y, 0.8f) && near(cam.RHSet.Forward.Z, 0.0f), "unlocked forward");
    check(near(cam.RHSet.Up.X, 0.0f) && near(cam.RHSet.Up.Y, 0.0f) && near(cam.RHSet.Up.Z, 1.0f), "up = world z");
    cam.PointAt(C3DVector(1.0f, 2.0f, 5.0f));        // straight up: (0,0,2) -> (0,0,1)
    check(near(cam.RHSet.Forward.Z, 1.0f) && near(cam.RHSet.Forward.X, 0.0f), "unlocked keeps z");
    cam.ViewVecZLocked = true;
    cam.PointAt(C3DVector(1.0f, 5.0f, 9.0f));        // z flattened: (0,3,0) -> (0,1,0)
    check(near(cam.RHSet.Forward.X, 0.0f) && near(cam.RHSet.Forward.Y, 1.0f) && near(cam.RHSet.Forward.Z, 0.0f), "locked flattens z");
    check(near(cam.RHSet.Up.Z, 1.0f), "locked up");
    cam.ViewVecZLocked = false;
    cam.PointAt(C3DVector(2.0f, 2.0f, 4.0f));        // (1,0,1)/sqrt2; up = z - f*(f.z) = (-0.5,0,0.5) -> (-0.707,0,0.707)
    check(near(cam.RHSet.Forward.X, 0.70710678f) && near(cam.RHSet.Forward.Z, 0.70710678f), "diagonal forward");
    check(near(cam.RHSet.Up.X, -0.70710678f) && near(cam.RHSet.Up.Z, 0.70710678f), "orthogonal up");
    if (fails) { puts("00a0c2b0_TEST FAIL"); return 1; }
    puts("00a0c2b0_TEST PASS");
    return 0;
}