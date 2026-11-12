// TEST: CGameCameraManager::IsPosCameraInsideThing(const CThing& thing, const C3DVector& pos) const -- the camera is inside the capsule from the thing up to 1.2x its eye height, 1.8x its collision radius wide
// Behaviour test for 0x00697490: the row body verbatim over a portable C3DVector with the members the shared
// engine/C3DVector.h gives it (GetMagnitude, Dot, GetSquaredMagnitude, +, -, * float, *=), and stub callees.
// C++03; builds with VC7.1 /Od and g++.
#include <stdio.h>
#include <string.h>
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
    float GetMagnitude() const { return (float)sqrt(GetSquaredMagnitude()); }
};
class CCharString { public: const char* Text; CCharString(const char* text, long length = -1) : Text(text) { (void)length; } };
class CRightHandedSet { public: C3DVector Forward; C3DVector Up; };
static int g_bad;
static C3DVector g_eye;
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector& position, CRightHandedSet& orientation, float scale) const
    {
        if (strcmp(name.Text, "EYE_SET_R") != 0 || zero1 != 0 || zero2 != 0 || scale != 1.0f) g_bad = 1;
        position = g_eye; orientation.Forward = C3DVector(0, 1, 0); orientation.Up = C3DVector(0, 0, 1);
        return true;
    }
};
}
class CTCPhysicsBase { public: float Radius; float GetRadius() const { return Radius; } };
class CThing { public:
    CTCPhysicsBase* PhysicsTC;
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() const { return PTCGraphicAppearance; }
    C3DVector Position;
    const C3DVector& GetPos() const { return Position; }
};
const float g_NormaliseEpsilon = 0.0001f;
static inline bool InRange(float value, float low, float high) { return value > low && value < high; }
class CGameCameraManager { public:
    bool IsPosCameraInsideThing(const CThing& thing, const C3DVector& pos) const;
};
// ---- row body (0x00697490) ----
bool CGameCameraManager::IsPosCameraInsideThing(const CThing& thing, const C3DVector& pos) const
{
    C3DVector eye;
    CRightHandedSet orientation;
    thing.GetTCGraphicAppearance()->GetDummyObject("EYE_SET_R", 0, 0, eye, orientation, 1.0f);
    C3DVector base = thing.GetPos();
    C3DVector top = base + (eye - base) * 1.2f;
    float radius = thing.GetPhysicsTC()->GetRadius() * 1.8f;
    C3DVector axis = top - base;
    float height = axis.GetMagnitude();
    axis *= (1.0f / height);
    C3DVector offset = pos - base;
    double along = offset.Dot(axis);
    if (InRange(along, -g_NormaliseEpsilon, height)) {
        C3DVector perp = offset - axis * (float)along;
        if (perp.GetSquaredMagnitude() < radius * radius)
            return true;
    }
    return false;
}
int main()
{
    CTCPhysicsBase physics; physics.Radius = 1.0f;          // capsule radius 1.8
    NGraphicAppearance::CTCGraphicAppearance appearance;
    CThing thing; thing.PhysicsTC = &physics; thing.PTCGraphicAppearance = &appearance; thing.Position = C3DVector(10, 20, 0);
    g_eye = C3DVector(10, 20, 2);                       // axis 2.4 high
    CGameCameraManager manager;
    bool ok = manager.IsPosCameraInsideThing(thing, C3DVector(11, 20, 1))      // beside the axis, inside the radius
        && !manager.IsPosCameraInsideThing(thing, C3DVector(12, 20, 1))        // 2 away: outside 1.8
        && manager.IsPosCameraInsideThing(thing, C3DVector(10, 21.5f, 2.3f))   // near the top
        && !manager.IsPosCameraInsideThing(thing, C3DVector(10, 20, 2.6f))     // above the top
        && !manager.IsPosCameraInsideThing(thing, C3DVector(10, 20, -0.5f))    // below the base
        && !g_bad;
    g_eye = C3DVector(13, 24, 0);                       // a tilted capsule (axis along 3,4,0 * 1.2 = 6 long)
    ok = ok && manager.IsPosCameraInsideThing(thing, C3DVector(13, 24, 1.0f))
        && !manager.IsPosCameraInsideThing(thing, C3DVector(10, 20, 2.0f))     // at the base, 2 above: outside 1.8
        && !manager.IsPosCameraInsideThing(thing, C3DVector(7, 12, 0));        // behind the base
    puts(ok ? "00697490_TEST PASS" : "00697490_TEST FAIL");
    return ok ? 0 : 1;
}