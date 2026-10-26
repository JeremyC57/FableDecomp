// Behaviour test for 0075c420: src/0075c420.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCCameraPointDefinitionBase::TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCameraPointDefinitionBase::TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const -- for a camera point using relative coordinates: express the position relative to the point's base in the given axes and move it to the origin; otherwise return it unchanged
// Leaf: TransformPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C420, length 205 bytes (+ int3 padding), `ret 10h` (hidden result + 3).
// Callees: CTCCameraPointDefinitionBase::TransformVector(const C3DVector&, const CRightHandedSet*) const @ 0x0075C280 (the
// orientation step; C3DVector by hidden pointer).
// Layout = Ego_r.pdb CTCCameraPointDefinitionBase (64 bytes): CoordBase +0x16 and CoordAxis +0x22 are unaligned in the PDB, so
// the class is packed; UsingRelativeCoords +0x3A, UsingRelativeOrientation +0x3B.
// Shape: the origin defaults to a zero vector (immediate float zeros) copied from the argument when given; the delta to the base
// is a local that the rotated vector is assigned back into (dword copy from the call's hidden result); the sum is built member by member into the result.
// Fix (retry 10): TransformVector (0x0075C280, same TU, C3DVector by hidden pointer) has a same-TU stand-in body; with only
// the declaration VC7.1 scheduled the result's word copies differently around the first fadd.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& b) const
    {
        return C3DVector(X - b.X, Y - b.Y, Z - b.Z);
    }
    C3DVector operator+(const C3DVector& b) const
    {
        return C3DVector(X + b.X, Y + b.Y, Z + b.Z);
    }
};
class CRightHandedSet;
#pragma pack(push, 1)
class CTCCameraPointDefinitionBase {
public:
    virtual void v00();
    void*   ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    CutInto;                            // +0x0c
    bool    CutOutOf;                           // +0x0d
    bool    TestAngleBeforeActivation;          // +0x0e
    unsigned char _pad0f;
    float   FOV;                                // +0x10
    bool    SelfTerminate;                      // +0x14
    bool    HeroIsSubject;                      // +0x15
    C3DVector CoordBase;                        // +0x16
    unsigned char CoordAxis[0x3a - 0x22];       // +0x22 CRightHandedSet
    bool    UsingRelativeCoords;                // +0x3a
    bool    UsingRelativeOrientation;           // +0x3b
    bool    Activated;                          // +0x3c
    C3DVector TransformVector(const C3DVector& vector, const CRightHandedSet* axes) const;   // 0x0075C280
    C3DVector TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const;
};
#pragma pack(pop)

float RotateComponent_s7(const void* axes, float x, float y, float z, long row);   // stand-in: one row of the rotation
#pragma auto_inline(off)
// Same-TU stand-in for TransformVector (0x0075C280): unchanged when the orientation is not relative, otherwise each
// component rotated by the axes (the row products done outside the graded bytes); reads the vector through its const
// reference and writes only the hidden result.
__declspec(noinline) C3DVector CTCCameraPointDefinitionBase::TransformVector(const C3DVector& vector, const CRightHandedSet* axes) const
{
    if (!UsingRelativeOrientation)
        return vector;
    float x = RotateComponent_s7(axes, vector.X, vector.Y, vector.Z, 0);
    float y = RotateComponent_s7(axes, vector.X, vector.Y, vector.Z, 1);
    float z = RotateComponent_s7(axes, vector.X, vector.Y, vector.Z, 2);
    return C3DVector(x, y, z);
}
#pragma auto_inline(on)
C3DVector CTCCameraPointDefinitionBase::TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const
{
    if (!UsingRelativeCoords)
        return position;
    C3DVector offset(0.0f, 0.0f, 0.0f);
    if (origin)
        offset = *origin;
    C3DVector local = position - CoordBase;
    local = TransformVector(local, axes);
    return local + offset;
}

// ---- stubs and driver ----
static int g_rot = 0; static const void* g_axes = 0;
float RotateComponent_s7(const void* axes, float x, float y, float z, long row)
{ ++g_rot; g_axes = axes; return row == 0 ? y : row == 1 ? -x : z * 2.0f; }
void CTCCameraPointDefinitionBase::v00() {}
int main()
{
    static CTCCameraPointDefinitionBase cam; cam.CoordBase = C3DVector(1.0f, 2.0f, 3.0f);
    C3DVector pos(5.0f, 7.0f, 11.0f), origin(100.0f, 200.0f, 300.0f);
    cam.UsingRelativeCoords = false;
    C3DVector r = cam.TransformPosition(pos, &origin, 0); CHECK(r.X == 5.0f && r.Y == 7.0f && r.Z == 11.0f); CHECK(g_rot == 0);
    cam.UsingRelativeCoords = true; cam.UsingRelativeOrientation = false;
    r = cam.TransformPosition(pos, &origin, 0); CHECK(r.X == 104.0f && r.Y == 205.0f && r.Z == 308.0f); CHECK(g_rot == 0);
    r = cam.TransformPosition(pos, 0, 0); CHECK(r.X == 4.0f && r.Y == 5.0f && r.Z == 8.0f);
    cam.UsingRelativeOrientation = true;
    r = cam.TransformPosition(pos, &origin, (const CRightHandedSet*)&origin);
    CHECK(g_rot == 3); CHECK(g_axes == &origin); CHECK(r.X == 105.0f && r.Y == 196.0f && r.Z == 316.0f);
    if (fails == 0) printf("0075c420_TEST PASS\n"); else printf("0075c420_TEST FAIL\n");
    return fails;
}