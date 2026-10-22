// Behaviour test for 0075c280: src/0075c280.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCCameraPointDefinitionBase::TransformVector(const C3DVector&, const CRightHandedSet*)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCameraPointDefinitionBase::TransformVector(const C3DVector& v, const CRightHandedSet* axes) -- with relative orientation on, turn the vector about world Z by the flat angle from the given axes' forward (else the point's own) to the point's own forward; otherwise return it unchanged
// Leaf: ?TransformVector@CTCCameraPointDefinitionBase@@IBE?AVC3DVector@@ABV2@PBVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C280, 410 bytes, `ret 0Ch`.
// Callees: C3DVector::Normalise 0x00A14440 (x2; catalog "GetScaled" is propagated), _CIacos 0x00BFEBA2 (intrinsic acos),
//   C3DVector::Rotate(const C3DVector&, float) 0x00A13C60; constants -1.0 0x0122DEE0, 1.0 0x0122DED8, 0.0 0x0122DEDC,
//   1/(2*pi) 0x01237F08.
// Layout: CTCCameraPointDefinitionBase is byte-packed (Ego_r.pdb): CoordAxis CRightHandedSet +0x22 (Forward +0x2e),
//   UsingRelativeOrientation +0x3b.
// Shape: the axes are a CRightHandedSet copy (scalarised from CoordAxis: Forward.X through a slot, Forward.Y in st0, the
//   "dead default load"; the copy from the argument is a full 6-dword copy); the first flat vector is a copy of the set's
//   forward with Z cleared, the second the (x, y, 0) constructor; the dot product is written Z + X + Y (VC7.1 emits it as
//   Y, X, add, Z, add); the sign test is the Z of from x to.
extern "C" double __cdecl acos(double);
#pragma intrinsic(acos)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Normalise();                                        // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);         // 0x00A13C60
};
struct CRightHandedSet {
    C3DVector Up;                                            // +0x00
    C3DVector Forward;                                       // +0x0c
};

#pragma pack(push, 1)
class CTCCameraPointDefinitionBase {
public:
    void*           __vftable;                               // +0x00
    unsigned char   _pad04[0x0c - 0x04];
    bool            CutInto;                                 // +0x0c
    bool            CutOutOf;                                // +0x0d
    bool            TestAngleBeforeActivation;               // +0x0e
    unsigned char   _pad0f;
    float           FOV;                                     // +0x10
    bool            SelfTerminate;                           // +0x14
    bool            HeroIsSubject;                           // +0x15
    C3DVector       CoordBase;                               // +0x16
    CRightHandedSet CoordAxis;                               // +0x22
    bool            UsingRelativeCoords;                     // +0x3a
    bool            UsingRelativeOrientation;                // +0x3b
    bool            Activated;                               // +0x3c
    C3DVector TransformVector(const C3DVector& v, const CRightHandedSet* axes) const;
};
#pragma pack(pop)

C3DVector CTCCameraPointDefinitionBase::TransformVector(const C3DVector& v, const CRightHandedSet* axes) const
{
    if (!UsingRelativeOrientation)
        return v;
    C3DVector result = v;
    CRightHandedSet set = CoordAxis;
    if (axes)
        set = *axes;
    C3DVector from = set.Forward;
    from.Z = 0.0f;
    from.Normalise();
    C3DVector to(CoordAxis.Forward.X, CoordAxis.Forward.Y, 0.0f);
    to.Normalise();
    float dot = from.Z * to.Z + from.X * to.X + from.Y * to.Y;
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float angle = (float)acos(dot) * 0.15915493667125702f;
    float sign = 1.0f;
    if (from.X * to.Y - from.Y * to.X < 0.0f)
        sign = -1.0f;
    result.Rotate(C3DVector(0.0f, 0.0f, 1.0f), sign * angle);
    return result;
}

// ---- stubs ----
static int g_normalised = 0; static float g_stretch = 1.0f;
void C3DVector::Normalise()
{
    ++g_normalised;
    float r = g_stretch / (float)sqrt(X * X + Y * Y + Z * Z);
    X *= r; Y *= r; Z *= r;
}
static int g_rotates = 0; static C3DVector g_axis; static float g_angle = -99.0f;
void C3DVector::Rotate(const C3DVector& axis, float angle)
{
    ++g_rotates; g_axis = axis; g_angle = angle;
    double a = angle * 6.283185307179586, c = cos(a), s = sin(a);
    float x = X, y = Y;
    X = (float)(x * c - y * s); Y = (float)(x * s + y * c);
}
static int Near(float a, float b) { return fabs(a - b) < 1e-4f; }
static void Reset() { g_normalised = 0; g_rotates = 0; g_angle = -99.0f; g_axis = C3DVector(9, 9, 9); g_stretch = 1.0f; }
static CRightHandedSet Axes(float fx, float fy, float fz)
{
    CRightHandedSet set; set.Up = C3DVector(0, 0, 1); set.Forward = C3DVector(fx, fy, fz); return set;
}

int main()
{
    CTCCameraPointDefinitionBase point; memset(&point, 0, sizeof point);
    CHECK((char*)&point.CoordAxis.Forward - (char*)&point == 0x2e && (char*)&point.UsingRelativeOrientation - (char*)&point == 0x3b);
    point.CoordAxis = Axes(0, 1, 0);
    C3DVector v(2, 0, 3);
    // 1: relative orientation off: the vector comes back unchanged, nothing is called
    {
        Reset(); point.UsingRelativeOrientation = false;
        CRightHandedSet axes = Axes(1, 0, 0);
        C3DVector r = point.TransformVector(v, &axes);
        CHECK(r.X == 2 && r.Y == 0 && r.Z == 3 && g_normalised == 0 && g_rotates == 0);
    }
    point.UsingRelativeOrientation = true;
    // 2: no axes: the point's own forward against itself, a zero turn about world Z
    {
        Reset();
        C3DVector r = point.TransformVector(v, 0);
        CHECK(g_normalised == 2 && g_rotates == 1 && g_axis.X == 0 && g_axis.Y == 0 && g_axis.Z == 1.0f);
        CHECK(Near(g_angle, 0.0f) && Near(r.X, 2) && Near(r.Y, 0) && r.Z == 3);
    }
    // 3: axes forward +X (its Z ignored), own forward +Y: a quarter turn anticlockwise
    {
        Reset();
        CRightHandedSet axes = Axes(5, 0, 4);
        C3DVector r = point.TransformVector(v, &axes);
        CHECK(Near(g_angle, 0.25f) && Near(r.X, 0) && Near(r.Y, 2) && r.Z == 3);
    }
    // 4: own forward -Y: the cross product is negative, so a quarter turn clockwise
    {
        Reset(); point.CoordAxis = Axes(0, -3, 0);
        CRightHandedSet axes = Axes(1, 0, 0);
        point.TransformVector(v, &axes);
        CHECK(Near(g_angle, -0.25f));
    }
    // 5: opposite forwards: half a turn
    {
        Reset(); point.CoordAxis = Axes(-1, 0, 0);
        CRightHandedSet axes = Axes(1, 0, 0);
        point.TransformVector(v, &axes);
        CHECK(Near(g_angle, 0.5f));
    }
    // 6: a dot product just past +1 or -1 is clamped before acos
    {
        Reset(); g_stretch = 1.001f; point.CoordAxis = Axes(1, 0, 0);
        CRightHandedSet axes = Axes(1, 0, 0);
        point.TransformVector(v, &axes);
        CHECK(g_angle == 0.0f);
        Reset(); g_stretch = 1.001f; point.CoordAxis = Axes(-1, 0, 0);
        point.TransformVector(v, &axes);
        CHECK(Near(g_angle, 0.5f));
    }
    if (fails == 0) printf("0075c280_TEST PASS\n");
    return fails;
}