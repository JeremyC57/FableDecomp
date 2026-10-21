// Behaviour test for 00a52450: src/00a52450.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. C3DKeyframe::ToMatrix(CMatrix3x4& m, bool applyScaling) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// C3DKeyframe::ToMatrix(CMatrix3x4& m, bool applyScaling) const -- keyframe (quaternion + position + scale) to a 3x4 matrix: the inline quaternion-to-matrix with the position as translation, then each rotation row scaled by the matching scale component
// Leaf: ?ToMatrix@C3DKeyframe
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A52450, 447 bytes, `ret 8`. FableWin symbol ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix3x4@@_N@Z
//   (the catalog label CQuaternion::ToMatrix is the inlined ?ToMatrix@CQuaternion@@QBEXAAVCMatrix3x4@@ABVC3DVector@@@Z).
// Callees: none (CQuaternion::ToMatrix is inlined).
// Globals: 0.0f 0x0122DEDC, 2.0f 0x0122DCDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb C3DKeyframe (Rotation +0, Position C4DVector +0x10, Scaling C4DVector +0x20) and CMatrix3x4 (anonymous
//   union over the floats E11..E43); no shared engine headers exist for these classes.
// Shape: the quaternion-to-matrix is a __forceinline member taking the translation by const reference; it copies the four
//   components into locals x, y, z, w first (loads in X..W order), and the norm sums w*w + z*z + y*y + x*x. The translation
//   is a C3DVector temporary built from the position (its Z travels as an integer copy). The scale components are float
//   locals declared Z, Y, X, and the rows are scaled with `*=` (the rotation values are forwarded from the frame).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C4DVector { float X, Y, Z, W; };
struct CMatrix3x4 {
    union {
        struct { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
    };
};
struct CQuaternion {
    float X, Y, Z, W;
    __forceinline void ToMatrix(CMatrix3x4& m, const C3DVector& pos) const
    {
        float x = X, y = Y, z = Z, w = W;
        float n = w * w + z * z + y * y + x * x;
        float s = (n != 0.0f) ? 2.0f / n : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        m.E11 = 1.0f - (yy + zz);
        m.E12 = xy - wz;
        m.E13 = xz + wy;
        m.E21 = xy + wz;
        m.E22 = 1.0f - (zz + xx);
        m.E23 = yz - wx;
        m.E31 = xz - wy;
        m.E32 = yz + wx;
        m.E33 = 1.0f - (xx + yy);
        m.E41 = pos.X;
        m.E42 = pos.Y;
        m.E43 = pos.Z;
    }
};
struct C3DKeyframe {
    CQuaternion Rotation;
    C4DVector Position;
    C4DVector Scaling;
    void ToMatrix(CMatrix3x4& m, bool applyScaling) const;
};
void C3DKeyframe::ToMatrix(CMatrix3x4& m, bool applyScaling) const
{
    Rotation.ToMatrix(m, C3DVector(Position.X, Position.Y, Position.Z));
    if (applyScaling) {
        float sz = Scaling.Z, sy = Scaling.Y, sx = Scaling.X;
        m.E11 *= sx; m.E12 *= sx; m.E13 *= sx;
        m.E21 *= sy; m.E22 *= sy; m.E23 *= sy;
        m.E31 *= sz; m.E32 *= sz; m.E33 *= sz;
    }
}

// ---- stubs and driver ----
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
static void Ref(const C3DKeyframe& k, bool scale, float* r)
{
    float x = k.Rotation.X, y = k.Rotation.Y, z = k.Rotation.Z, w = k.Rotation.W;
    float n = x * x + y * y + z * z + w * w;
    float s = n != 0.0f ? 2.0f / n : 1.0f;
    r[0] = 1 - s * (y * y + z * z); r[1] = s * (x * y - w * z); r[2] = s * (x * z + w * y);
    r[3] = s * (x * y + w * z); r[4] = 1 - s * (x * x + z * z); r[5] = s * (y * z - w * x);
    r[6] = s * (x * z - w * y); r[7] = s * (y * z + w * x); r[8] = 1 - s * (x * x + y * y);
    r[9] = k.Position.X; r[10] = k.Position.Y; r[11] = k.Position.Z;
    if (scale) {
        for (int i = 0; i < 3; ++i) { r[i] *= k.Scaling.X; r[3 + i] *= k.Scaling.Y; r[6 + i] *= k.Scaling.Z; }
    }
}
static void Run(float qx, float qy, float qz, float qw, bool scale)
{
    C3DKeyframe k;
    k.Rotation.X = qx; k.Rotation.Y = qy; k.Rotation.Z = qz; k.Rotation.W = qw;
    k.Position.X = 5.0f; k.Position.Y = -6.5f; k.Position.Z = 7.25f; k.Position.W = 99.0f;
    k.Scaling.X = 2.0f; k.Scaling.Y = 3.0f; k.Scaling.Z = 0.5f; k.Scaling.W = 42.0f;
    CMatrix3x4 m; memset(&m, 0xCD, sizeof(m));
    k.ToMatrix(m, scale);
    float r[12]; Ref(k, scale, r);
    const float* e = &m.E11;
    for (int i = 0; i < 12; ++i) CHECK(Near(e[i], r[i]));
}
int main()
{
    Run(0.0f, 0.0f, 0.0f, 1.0f, false);           // identity rotation, translation only
    Run(0.0f, 0.0f, 0.0f, 1.0f, true);            // diagonal = scale
    Run(0.1825742f, 0.3651484f, 0.5477226f, 0.7302967f, false);   // unit quaternion
    Run(1.0f, 2.0f, 3.0f, 4.0f, true);            // non-unit (2/n), scaled
    Run(0.0f, 0.0f, 0.0f, 0.0f, true);            // zero norm: s = 1
    {   // scale off leaves rotation rows untouched by Scaling
        C3DKeyframe k; memset(&k, 0, sizeof(k)); k.Rotation.W = 1.0f; k.Scaling.X = k.Scaling.Y = k.Scaling.Z = 9.0f;
        CMatrix3x4 m; k.ToMatrix(m, false);
        CHECK(m.E11 == 1.0f && m.E22 == 1.0f && m.E33 == 1.0f && m.E12 == 0.0f);
    }
    if (fails == 0) printf("00a52450_TEST PASS\n");
    return fails;
}
