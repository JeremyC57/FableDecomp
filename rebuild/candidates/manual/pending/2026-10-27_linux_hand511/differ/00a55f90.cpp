// CMatrix3x4::SetFromVectorAndAngle(const C3DVector& axis, float angle) -- make this the rotation by `angle` (a fraction of a full turn) about the unit `axis`, with a zero translation
// Grade: DIFFER(259v259) -- two product operand orders and one scheduling run: retail `fld [Y] ; fmul [X]` (xy) and `fld [Z] ; fmul [X]` (xz), ours `fld [X] ; fmul [Y|Z]` whichever way the source writes them; retail zeroes Position (xor eax ; three dword stores) after the last `fmul st(2)`, ours interleaves it two instructions earlier
// Leaf: ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A55F90, 259 bytes, thiscall `ret 8`.
// Identity: both PDBs name `?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z` (void, thiscall, const C3DVector&,
//   float) and list no CMatrix3x4 constructor taking (const C3DVector&, float); its callers are C3DVector::Rotate
//   0x00A13C60 (which builds the matrix in its frame and reads the three axes back), CTCLook::OnAppearanceDraw (x4) and
//   two particle components. The landed 0x00A13C60 binds it as `??0CMatrix3x4@@QAE@ABVC3DVector@@M@Z` (a ctor that no
//   PDB has: label correction). Catalog label "Matrix_RotationAroundAxis" is descriptive.
// Callees: none (intrinsic fcos / fsin). Globals: 2*pi as a double 0x0124F2B8 (6.2831854820251465 = (double)6.2831855f).
// Shape: the standard axis-angle matrix written axis-component by axis-component (X of each axis, then Y, then Z: retail's
//   store order), each diagonal term `aa + c - aa*c` and each off-diagonal pair `ab - ab*c +/- s*k` (VC7.1 CSEs the
//   `ab - ab*c` halves); the products are taken once into locals.
#include <math.h>
#pragma intrinsic(sin, cos)

class C3DVector { public:
    float X, Y, Z;
};

class CMatrix3x4 { public:
    C3DVector XAxis;                                    // +0x00
    C3DVector YAxis;                                    // +0x0c
    C3DVector ZAxis;                                    // +0x18
    C3DVector Position;                                 // +0x24
    void SetFromVectorAndAngle(const C3DVector& axis, float angle);
};

void CMatrix3x4::SetFromVectorAndAngle(const C3DVector& axis, float angle)
{
    float a = (float)(angle * 6.2831854820251465);
    float c = (float)cos(a);
    float s = (float)sin(a);
    float xx = axis.X * axis.X;
    float yy = axis.Y * axis.Y;
    float zz = axis.Z * axis.Z;
    float xy = axis.Y * axis.X;
    float xz = axis.Z * axis.X;
    float yz = axis.Z * axis.Y;
    float sx = axis.X * s;
    float sy = axis.Y * s;
    float sz = axis.Z * s;
    XAxis.X = xx + c - xx * c;
    YAxis.X = xy - xy * c + sz;
    ZAxis.X = xz - xz * c - sy;
    XAxis.Y = xy - xy * c - sz;
    YAxis.Y = yy + c - yy * c;
    ZAxis.Y = yz - yz * c + sx;
    XAxis.Z = xz - xz * c + sy;
    YAxis.Z = yz - yz * c - sx;
    ZAxis.Z = zz + c - zz * c;
    Position.X = 0.0f;
    Position.Y = 0.0f;
    Position.Z = 0.0f;
}
