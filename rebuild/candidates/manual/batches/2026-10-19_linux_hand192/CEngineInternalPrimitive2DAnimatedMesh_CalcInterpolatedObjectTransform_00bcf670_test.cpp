#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
// CEngineInternalPrimitive2DAnimatedMesh::CalcInterpolatedObjectTransform(CMatrix3x4&, float, bool) const -- interpolate the world position and orientation at `t`, re-orthogonalise the orientation, pick the mesh scale (or 1) and build the object matrix about the object bounding-sphere centre
// Leaf: CalcInterpolatedObjectTransform
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCF670, 219 bytes (+ int3 padding), `ret 0Ch`. Mangled
//   ?CalcInterpolatedObjectTransform@CEngineInternalPrimitive2DAnimatedMesh@@UBEXAAVCMatrix3x4@@M_N@Z.
// Callees: CRightHandedSet::Interpolate(const CRightHandedSet&, float) const 0x00A23190 (returns the set by hidden pointer),
//   CRightHandedSet::MakePerpendicular 0x00A22E10, the 2D CalcObjectMatrix 0x00BEB6C0 (`__fastcall`: matrix in ecx, the centre
//   in edx, position, orientation and scale on the stack).
// Layout (Ego_r.pdb, retail agrees): CEngineInternalPrimitiveMeshBase::Scale +0x58; WorldPos (C3DPositionInterpolator, vptr
//   +0x84) StartPosition +0x88 / EndPosition +0x94; Orientation (C3DOrientationInterpolator, vptr +0xa0) StartOrientation
//   +0xa4 / EndOrientation +0xbc (CRightHandedSet = Up, Forward); the 2D class's ObjectBoundingSphere centre is read at
//   +0x140 in retail (Ego_r.pdb / engine/CEngineInternalPrimitive2DAnimatedMesh.h: +0x144, the flattened header has no usable
//   base layout, so `this` is a local model; the 4-byte shift comes from the base's 12-byte retail containers).
// Shape: position = start + (end - start) * t through inline C3DVector operators (by-value results, no user copy ctor);
//   the scale is a named float local (homed in the dead bool slot); the centre is a C3DVector local built member-wise after the
//   scale (integer moves); the helpers take references to the frame locals. Twin of 0x00BCEF80.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

struct CRightHandedSet {
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
    CRightHandedSet Interpolate(const CRightHandedSet& other, float t) const;   // 0x00A23190
    void MakePerpendicular();                          // 0x00A22E10
};

struct CMatrix3x4 { float E[12]; };

void __fastcall CalcObjectMatrix2D(CMatrix3x4& out, const C3DVector& centre, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00BEB6C0

struct C3DPositionInterpolator {
    void*     __vftable;
    C3DVector StartPosition;                           // +0x04
    C3DVector EndPosition;                             // +0x10
};
struct C3DOrientationInterpolator {
    void*           __vftable;
    CRightHandedSet StartOrientation;                  // +0x04
    CRightHandedSet EndOrientation;                    // +0x1c
};

struct CEngineInternalPrimitiveAnimatedMesh {
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& out, float t, bool applyScale) const;
    unsigned char              _pad04[0x58 - 4];
    float                      Scale;                  // +0x58
    unsigned char              _pad5c[0x84 - 0x5c];
    C3DPositionInterpolator    WorldPos;               // +0x84
    C3DOrientationInterpolator Orientation;            // +0xa0
};

struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
};

struct CEngineInternalPrimitive2DAnimatedMesh : CEngineInternalPrimitiveAnimatedMesh {
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& out, float t, bool applyScale) const;
    unsigned char     _pad0d4[0x140 - 0xd4];
    C3DBoundingSphere ObjectBoundingSphere;            // +0x140 (retail)
};

void CEngineInternalPrimitive2DAnimatedMesh::CalcInterpolatedObjectTransform(CMatrix3x4& out, float t, bool applyScale) const
{
    C3DVector position = WorldPos.StartPosition + (WorldPos.EndPosition - WorldPos.StartPosition) * t;
    CRightHandedSet orientation = Orientation.StartOrientation.Interpolate(Orientation.EndOrientation, t);
    orientation.MakePerpendicular();
    float scale;
    if (applyScale)
        scale = Scale;
    else
        scale = 1.0f;
    C3DVector centre(ObjectBoundingSphere.Center.X, ObjectBoundingSphere.Center.Y, ObjectBoundingSphere.Center.Z);
    CalcObjectMatrix2D(out, centre, position, orientation, scale);
}

// ---- test doubles and driver ----
void CEngineInternalPrimitiveAnimatedMesh::CalcInterpolatedObjectTransform(CMatrix3x4&, float, bool) const { }   // base slot (not under test)
static CMatrix3x4* g_out; static C3DVector g_pos, g_centre; static CRightHandedSet g_rhs; static float g_scale; static float g_t; static int g_perp = 0;
CRightHandedSet CRightHandedSet::Interpolate(const CRightHandedSet& other, float t) const
{
    g_t = t; CRightHandedSet r; r.Up = Up + (other.Up - Up) * t; r.Forward = Forward + (other.Forward - Forward) * t; return r;
}
void CRightHandedSet::MakePerpendicular() { ++g_perp; Up.X += 100.0f; }
void __fastcall CalcObjectMatrix2D(CMatrix3x4& out, const C3DVector& centre, const C3DVector& position, const CRightHandedSet& orientation, float scale) { g_out = &out; g_centre = centre; g_pos = position; g_rhs = orientation; g_scale = scale; }
int main()
{
    static CEngineInternalPrimitive2DAnimatedMesh m;
    m.Scale = 2.5f;
    m.WorldPos.StartPosition = C3DVector(0.0f, 10.0f, -4.0f);
    m.WorldPos.EndPosition = C3DVector(8.0f, 20.0f, 4.0f);
    m.Orientation.StartOrientation.Up = C3DVector(0.0f, 0.0f, 1.0f); m.Orientation.StartOrientation.Forward = C3DVector(0.0f, 1.0f, 0.0f);
    m.Orientation.EndOrientation.Up = C3DVector(4.0f, 0.0f, 1.0f); m.Orientation.EndOrientation.Forward = C3DVector(0.0f, 1.0f, 8.0f);
    m.ObjectBoundingSphere.Center = C3DVector(7.0f, 8.0f, 9.0f);
    CMatrix3x4 out;
    m.CalcInterpolatedObjectTransform(out, 0.25f, true);
    bool ok = g_out == &out && g_pos.X == 2.0f && g_pos.Y == 12.5f && g_pos.Z == -2.0f && g_scale == 2.5f && g_t == 0.25f
        && g_rhs.Up.Z == 1.0f && g_rhs.Forward.Z == 2.0f;
    m.CalcInterpolatedObjectTransform(out, 1.0f, false);
    ok = ok && g_scale == 1.0f && g_pos.X == 8.0f && g_pos.Z == 4.0f && g_perp == 2 && g_rhs.Up.X == 104.0f && g_centre.X == 7.0f && g_centre.Y == 8.0f && g_centre.Z == 9.0f;
    if (!ok) { std::printf("00bcf670_TEST FAIL\n"); return 1; }
    std::printf("00bcf670_TEST PASS\n");
    return 0;
}
