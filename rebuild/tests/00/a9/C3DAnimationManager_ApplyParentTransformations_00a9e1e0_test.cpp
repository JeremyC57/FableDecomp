// Behaviour test for 0x00A9E1E0 C3DAnimationManager::ApplyParentTransformations(const C3DKeyframe*, CMatrix4x4*, const C3DMesh2&, bool) const
// A plain C++ copy of the row's logic (the x87 arm of CMatrix4x4::AlignedSetMultiply and, as the SSE stand-in, the
// FABLE_PORTABLE twin's pairwise sums), checked against independent expectations: identity / axis rotations, translations,
// parent chains, the transposed result equal to the transpose of the row-vector result, a scaled quaternion giving the same
// matrix (2 / |q|^2), and the zero quaternion taking the s = 1 branch.
#include <cstdio>
#include <cmath>

class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
class C4DVector { public: float X, Y, Z, W; };
class CMatrix4x4 { public:
    float Data[4][4];
    float& operator()(int r, int c) { return Data[r][c]; }
    float operator()(int r, int c) const { return Data[r][c]; }
    void AlignedSetMultiply(const CMatrix4x4& a, const CMatrix4x4& b, bool sse);
    void AlignedSetMultiplyTransposed(const CMatrix4x4& a, const CMatrix4x4& b, bool sse) { AlignedSetMultiply(b, a, sse); }
};
static float GFSqr(float v) { return v * v; }
class CQuaternion { public:
    float X, Y, Z, W;
    void ToMatrix(CMatrix4x4& dest, const C3DVector& t) const {
        float d = GFSqr(X) + GFSqr(Y) + GFSqr(Z) + GFSqr(W);
        float s = (d != 0.0f) ? 2.0f / d : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        dest(0,0) = 1.0f - (yy + zz); dest(0,1) = xy - wz; dest(0,2) = xz + wy; dest(0,3) = 0.0f;
        dest(1,0) = xy + wz; dest(1,1) = 1.0f - (xx + zz); dest(1,2) = yz - wx; dest(1,3) = 0.0f;
        dest(2,0) = xz - wy; dest(2,1) = yz + wx; dest(2,2) = 1.0f - (xx + yy); dest(2,3) = 0.0f;
        dest(3,0) = t.X; dest(3,1) = t.Y; dest(3,2) = t.Z; dest(3,3) = 1.0f;
    }
    void ToMatrixTransposed(CMatrix4x4& dest, const C3DVector& t) const {
        float d = GFSqr(X) + GFSqr(Y) + GFSqr(Z) + GFSqr(W);
        float s = (d != 0.0f) ? 2.0f / d : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        dest(0,0) = 1.0f - (yy + zz); dest(0,1) = xy + wz; dest(0,2) = xz - wy; dest(0,3) = t.X;
        dest(1,0) = xy - wz; dest(1,1) = 1.0f - (xx + zz); dest(1,2) = yz + wx; dest(1,3) = t.Y;
        dest(2,0) = xz + wy; dest(2,1) = yz - wx; dest(2,2) = 1.0f - (xx + yy); dest(2,3) = t.Z;
        dest(3,0) = 0.0f; dest(3,1) = 0.0f; dest(3,2) = 0.0f; dest(3,3) = 1.0f;
    }
};
class C3DKeyframe { public:
    CQuaternion Rotation; C4DVector Position; C4DVector Scaling;
    C3DVector GetPosition() const { return C3DVector(Position.X, Position.Y, Position.Z); }
    void ToMatrix(CMatrix4x4& m, bool) const { Rotation.ToMatrix(m, GetPosition()); }
    void ToMatrixTransposed(CMatrix4x4& m, bool) const { Rotation.ToMatrixTransposed(m, GetPosition()); }
};
void CMatrix4x4::AlignedSetMultiply(const CMatrix4x4& a, const CMatrix4x4& b, bool sse)
{
    if (sse) {   // the FABLE_PORTABLE twin of GF_SSE_MATRIX_MULTIPLY: (a0*b0 + a1*b1) + (a2*b2 + a3*b3), a and b read before dst
        float out[4][4];
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                out[r][c] = (a(r,0) * b(0,c) + a(r,1) * b(1,c)) + (a(r,2) * b(2,c) + a(r,3) * b(3,c));
        for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) Data[r][c] = out[r][c];
    } else {
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                (*this)(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c);
    }
}
class C3DBone2 { public: unsigned long NameCRC; long Parent; long OriginalNoChildren; float LocalizationMatrix[12];
                  long GetParent() const { return Parent; } };
class C3DMesh2 { public: unsigned long BoneCount; C3DBone2* Bones;
                  unsigned long GetBoneCount() const { return BoneCount; }
                  const C3DBone2* GetBone(unsigned long i) const { return &Bones[i]; } };

// the row's logic (g_SSEAvailable selects the kernel inside AlignedSetMultiply)
static void ApplyParentTransformations(const C3DKeyframe* keyframes, CMatrix4x4* matrices, const C3DMesh2& mesh, bool transpose, bool sse)
{
    unsigned long bone_count = mesh.GetBoneCount();
    unsigned long i;
    if (transpose) {
        for (i = 0; i < bone_count; ++i) {
            const C3DBone2* bone = mesh.GetBone(i);
            long parent = bone->GetParent();
            if (parent < 0)
                keyframes[i].ToMatrixTransposed(matrices[i], false);
            else {
                CMatrix4x4 local;
                keyframes[i].ToMatrixTransposed(local, false);
                matrices[i].AlignedSetMultiplyTransposed(local, matrices[parent], sse);
            }
        }
    } else {
        for (i = 0; i < bone_count; ++i) {
            const C3DBone2* bone = mesh.GetBone(i);
            long parent = bone->GetParent();
            if (parent < 0)
                keyframes[i].ToMatrix(matrices[i], false);
            else {
                CMatrix4x4 local;
                keyframes[i].ToMatrix(local, false);
                matrices[i].AlignedSetMultiply(local, matrices[parent], sse);
            }
        }
    }
}

static int fails = 0;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++fails; } }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
// row-vector transform of a point (x, y, z, 1) by m
static void xform(const CMatrix4x4& m, float x, float y, float z, float* o)
{
    for (int c = 0; c < 3; ++c) o[c] = x * m.Data[0][c] + y * m.Data[1][c] + z * m.Data[2][c] + m.Data[3][c];
}
static C3DKeyframe kf(float qx, float qy, float qz, float qw, float px, float py, float pz)
{
    C3DKeyframe k; k.Rotation.X = qx; k.Rotation.Y = qy; k.Rotation.Z = qz; k.Rotation.W = qw;
    k.Position.X = px; k.Position.Y = py; k.Position.Z = pz; k.Position.W = 0.0f;
    k.Scaling.X = k.Scaling.Y = k.Scaling.Z = 1.0f; k.Scaling.W = 0.0f; return k;
}

int main()
{
    const float h = 0.70710678f;   // sin / cos of 45 degrees: a 90-degree rotation quaternion component
    C3DKeyframe keys[4];
    keys[0] = kf(0, 0, 0, 1, 1, 2, 3);          // root: identity at (1,2,3)
    keys[1] = kf(0, 0, h, h, 10, 0, 0);          // child of 0: 90 degrees about Z, offset (10,0,0)
    keys[2] = kf(0, 0, 0, 2, 0, 5, 0);           // child of 1: scaled identity quaternion (normalised by 2/|q|^2), offset (0,5,0)
    keys[3] = kf(0, 0, 0, 0, 7, 8, 9);           // second root: zero quaternion -> s = 1 branch
    C3DBone2 bones[4];
    long parents[4] = { -1, 0, 1, -1 };
    for (int b = 0; b < 4; ++b) { bones[b].NameCRC = b; bones[b].Parent = parents[b]; bones[b].OriginalNoChildren = 0; }
    C3DMesh2 mesh; mesh.BoneCount = 4; mesh.Bones = bones;

    for (int sse = 0; sse < 2; ++sse) {
        CMatrix4x4 m[4], t[4];
        ApplyParentTransformations(keys, m, mesh, false, sse != 0);
        ApplyParentTransformations(keys, t, mesh, true, sse != 0);
        float o[3];
        // root: translation only
        xform(m[0], 0, 0, 0, o); check(near(o[0], 1) && near(o[1], 2) && near(o[2], 3), "root translation");
        // bone 1: local point (1,0,0) rotates (row-vector form of the quaternion matrix) then offsets, then the parent's offset
        xform(m[1], 0, 0, 0, o); check(near(o[0], 11) && near(o[1], 2) && near(o[2], 3), "bone1 origin");
        xform(m[1], 1, 0, 0, o); check(near(o[0], 11) && near(o[1], 1) && near(o[2], 3), "bone1 x axis");
        // bone 2: offset (0,5,0) through bone 1's rotation, the normalised identity rotation
        xform(m[2], 0, 0, 0, o); check(near(o[0], 16) && near(o[1], 2) && near(o[2], 3), "bone2 origin");
        check(near(m[2].Data[0][0], m[1].Data[0][0]) && near(m[2].Data[0][1], m[1].Data[0][1]), "bone2 rotation = parent's");
        // second root, zero quaternion: s = 1 and every product 0 -> identity rotation
        check(near(m[3].Data[0][0], 1) && near(m[3].Data[1][1], 1) && near(m[3].Data[2][2], 1) && near(m[3].Data[3][0], 7)
              && near(m[3].Data[3][2], 9) && near(m[3].Data[0][3], 0) && near(m[3].Data[3][3], 1), "zero quaternion");
        // the transposed pass is the transpose of the row-vector pass, bone for bone
        bool tr = true;
        for (int b = 0; b < 4; ++b) for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) tr = tr && near(t[b].Data[r][c], m[b].Data[c][r]);
        check(tr, "transposed == transpose");
    }
    // the two kernels agree
    {
        CMatrix4x4 a[4], b[4];
        ApplyParentTransformations(keys, a, mesh, false, false);
        ApplyParentTransformations(keys, b, mesh, false, true);
        bool same = true;
        for (int k = 0; k < 4; ++k) for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) same = same && near(a[k].Data[r][c], b[k].Data[r][c]);
        check(same, "x87 arm == SSE twin");
    }
    // an empty mesh writes nothing
    {
        CMatrix4x4 m[1]; m[0].Data[0][0] = 42.0f; C3DMesh2 empty; empty.BoneCount = 0; empty.Bones = bones;
        ApplyParentTransformations(keys, m, empty, true, false); check(m[0].Data[0][0] == 42.0f, "empty mesh");
    }
    if (fails) { std::printf("00a9e1e0_TEST FAIL\n"); return 1; }
    std::printf("00a9e1e0_TEST PASS\n");
    return 0;
}