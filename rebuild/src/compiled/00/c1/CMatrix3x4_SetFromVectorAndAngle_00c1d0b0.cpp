// CMatrix3x4::SetFromVectorAndAngle(const C3DVector& axis, float angle) -- set the matrix to the rotation by `angle` radians about the unit `axis` (Rodrigues form: diagonal aa + (1 - aa) cos, off-diagonal ab - ab cos -/+ s c) with a zero translation row
// Leaf: ?SetFromVectorAndAngle@CMatrix3x4@@QAIXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1D0B0, length 257 bytes, `this` in ecx, the axis in edx, the angle on the
//   stack, `ret 4`; no direct callers found (like the landed PostMultiply 0x00C1CF20 just before it).
// Identity: Ego_r.pdb and FableWin.pdb both name `?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z` (the only
//   CMatrix3x4 member taking a C3DVector and a float), and the body is exactly that: cos/sin of the float, the axis
//   squares and pair products, then E11 = xx + (1 - xx) c, E21/E12 = xy - xy c +/- s z, E31/E13 = xz - xz c -/+ s y,
//   E32/E23 = yz - yz c +/- s x, E41..E43 = 0. The catalog label "Matrix_RotateAroundAxis" is descriptive and was right
//   about the meaning. Convention: the TLC math module passes the C3DVector in edx (no stack argument but the float), as
//   the landed PreMultiply 0x00C1CDA0 / PostMultiply 0x00C1CF20 do; modelled `__fastcall` like them, so the compiled
//   symbol is the QAI spelling of the PDB's QAE name (see NOTES, Review at landing).
// Layout: CMatrix3x4 (PDB, 0x30 bytes): E11..E13, E21..E23, E31..E33, E41..E43 (row 4 is the translation).
// Callees: none (cos/sin are the /O2 x87 intrinsics: `fld [esp+4] ; fcos ; fld [esp+4] ; fsin`). Constant 1.0f @ 0x0122DED8.
#include <math.h>

class C3DVector { public: float X, Y, Z; };            // 12 bytes, 1-aligned in retail

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    void __fastcall SetFromVectorAndAngle(const C3DVector& axis, float angle);
};

void __fastcall CMatrix3x4::SetFromVectorAndAngle(const C3DVector& axis, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    float xx = axis.X * axis.X;
    float yy = axis.Y * axis.Y;
    float zz = axis.Z * axis.Z;
    float xy = axis.X * axis.Y;
    float xz = axis.X * axis.Z;
    float yz = axis.Y * axis.Z;
    float sx = s * axis.X;
    float sy = s * axis.Y;
    float sz = s * axis.Z;

    E11 = (1.0f - xx) * c + xx;
    E21 = xy - xy * c + sz;
    E31 = xz - xz * c - sy;
    E12 = xy - xy * c - sz;
    E22 = (1.0f - yy) * c + yy;
    E32 = yz - yz * c + sx;
    E13 = xz - xz * c + sy;
    E23 = yz - yz * c - sx;
    E33 = (1.0f - zz) * c + zz;
    E41 = E42 = E43 = 0.0f;
}