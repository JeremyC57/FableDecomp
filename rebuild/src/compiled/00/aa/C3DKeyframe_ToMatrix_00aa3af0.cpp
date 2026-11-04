// C3DKeyframe::ToMatrix(CMatrix4x4& m, bool applyScaling) const -- keyframe (quaternion + position) to a 4x4 matrix: the inline quaternion-to-matrix with the position as the translation column and a 0,0,0,1 last row; this build does not read applyScaling
// Leaf: ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA3AF0, 346 bytes, thiscall `ret 8` (matrix reference + bool, the bool unread).
// Identity: FableWin.pdb names exactly two 4x4 conversions, ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z and
//   the inline ?ToMatrix@CQuaternion@@QBEXAAVCMatrix4x4@@ABVC3DVector@@@Z; this body reads the 48-byte keyframe (Rotation +0,
//   Position +0x10) through `this`, takes two stack arguments and inlines the quaternion conversion -- the 4x4 twin of the
//   landed C3DKeyframe::ToMatrix(CMatrix3x4&, bool) 0x00A52450 without its scaling block. No callers in the retail image.
// Callees: none (CQuaternion::ToMatrix is inlined).
// Globals: 0.0f 0x0122DEDC, 2.0f 0x0122DCDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb C3DKeyframe and CMatrix4x4 (anonymous union over E11..E44); no shared engine headers exist for them.
// Shape: the inline 4x4 conversion writes every element through the FableWin inline accessor
//   ??RCMatrix4x4@@QAEAAMHH@Z (float& operator()(int row, int col), over a float[4][4] view of the union). With the
//   named members E11..E44 VC7.1 hoists the six integer stores (E34 = pos.Z, the 0,0,0 last row, E44 = 1.0f) up to
//   just after the E11 store; through the returned float& it keeps them in source order after E33, as retail does
//   (the matching lever after 4 named-member grades: chained zeros, reversed zero order, named pos local).
#include "engine/C3DVector.h"
class C4DVector { public: float X, Y, Z, W; };
class CMatrix4x4 { public:
    union {
        struct { float E11, E12, E13, E14, E21, E22, E23, E24, E31, E32, E33, E34, E41, E42, E43, E44; };
        float M[4][4];
    };
    float& operator()(int row, int col) { return M[row][col]; }
};
class CQuaternion { public:
    float X, Y, Z, W;
    __forceinline void ToMatrix(CMatrix4x4& m, const C3DVector& pos) const
    {
        float x = X, y = Y, z = Z, w = W;
        float n = w * w + z * z + y * y + x * x;
        float s = (n != 0.0f) ? 2.0f / n : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        m(0, 0) = 1.0f - (yy + zz);
        m(0, 1) = xy + wz;
        m(0, 2) = xz - wy;
        m(0, 3) = pos.X;
        m(1, 0) = xy - wz;
        m(1, 1) = 1.0f - (zz + xx);
        m(1, 2) = yz + wx;
        m(1, 3) = pos.Y;
        m(2, 0) = xz + wy;
        m(2, 1) = yz - wx;
        m(2, 2) = 1.0f - (xx + yy);
        m(2, 3) = pos.Z;
        m(3, 0) = 0.0f;
        m(3, 1) = 0.0f;
        m(3, 2) = 0.0f;
        m(3, 3) = 1.0f;
    }
};
class C3DKeyframe { public:
    CQuaternion Rotation;
    C4DVector Position;
    C4DVector Scaling;
    void ToMatrix(CMatrix4x4& m, bool applyScaling) const;
};
void C3DKeyframe::ToMatrix(CMatrix4x4& m, bool applyScaling) const
{
    Rotation.ToMatrix(m, C3DVector(Position.X, Position.Y, Position.Z));
}