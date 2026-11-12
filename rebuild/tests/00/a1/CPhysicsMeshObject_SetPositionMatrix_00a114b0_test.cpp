// Behaviour test for 0x00A114B0 CPhysicsMeshObject::SetPositionMatrix: a copy of the model (vector, right-handed set, 3x4 matrix,
// the object's matrix / scale members) and of the function's logic, checked on an axis-aligned frame, on a skewed frame against
// hand-computed values, and for the scale / reciprocal members.
#include <cstdio>
struct TVector {
    float X, Y, Z;
    TVector() {}
    TVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const TVector operator^(const TVector& o) const { return TVector((Y * o.Z - Z * o.Y), (Z * o.X - X * o.Z), (X * o.Y - Y * o.X)); }
};
struct TRightHandedSet {
    TVector Up, Forward;
    TVector GetUp() const { return Up; }
    TVector GetForward() const { return Forward; }
};
struct TMatrix3x4 {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void SetIdentity() { E12 = E13 = E21 = E23 = E31 = E32 = E41 = E42 = E43 = 0.0f; E11 = E22 = E33 = 1.0f; }
    void PostTranslate(const TVector& v) { E41 += v.X; E42 += v.Y; E43 += v.Z; }
    void PreScale(float s) { E11 *= s; E12 *= s; E13 *= s; E21 *= s; E22 *= s; E23 *= s; E31 *= s; E32 *= s; E33 *= s; }
};
struct TPhysicsMeshObject {
    void* VTable; void* Mesh;
    TMatrix3x4 PositionMatrix;
    float Scale, OO_Scale;
    void SetPositionMatrix(const TVector& position, const TRightHandedSet& axes, float scale)
    {
        TVector up = axes.GetUp();
        TVector forward = axes.GetForward();
        const TVector& right = axes.GetForward() ^ axes.GetUp();
        PositionMatrix.SetIdentity();
        PositionMatrix.PostTranslate(TVector(position));
        PositionMatrix.E11 = -right.X; PositionMatrix.E12 = -right.Y; PositionMatrix.E13 = -right.Z;
        PositionMatrix.E21 = -forward.X; PositionMatrix.E22 = -forward.Y; PositionMatrix.E23 = -forward.Z;
        PositionMatrix.E31 = up.X; PositionMatrix.E32 = up.Y; PositionMatrix.E33 = up.Z;
        PositionMatrix.PreScale(scale);
        Scale = scale;
        OO_Scale = 1.0f / scale;
    }
};
static bool Same(const TMatrix3x4& m, const float* e)
{
    const float* p = &m.E11;
    for (int i = 0; i < 12; ++i) if (p[i] != e[i]) return false;
    return true;
}
int main()
{
    bool ok = sizeof(TMatrix3x4) == 48 && sizeof(TPhysicsMeshObject) == 0x40;
    TPhysicsMeshObject o;
    for (int i = 0; i < 12; ++i) (&o.PositionMatrix.E11)[i] = 99.0f;   // stale contents must not survive
    // 1. axis-aligned frame: up = +Z, forward = +Y  =>  right = forward ^ up = +X; rows -X, -Y, +Z, scaled by 2; translation unscaled.
    TRightHandedSet a; a.Up = TVector(0.0f, 0.0f, 1.0f); a.Forward = TVector(0.0f, 1.0f, 0.0f);
    o.SetPositionMatrix(TVector(3.0f, 4.0f, 5.0f), a, 2.0f);
    const float e1[12] = { -2.0f, 0.0f, 0.0f,  0.0f, -2.0f, 0.0f,  0.0f, 0.0f, 2.0f,  3.0f, 4.0f, 5.0f };
    ok = ok && Same(o.PositionMatrix, e1) && o.Scale == 2.0f && o.OO_Scale == 0.5f;
    // 2. skewed frame (exact in binary): up = (1,2,3), forward = (4,5,6)
    //    right = forward ^ up = (5*3-6*2, 6*1-4*3, 4*2-5*1) = (3,-6,3); scale 0.5.
    TRightHandedSet b; b.Up = TVector(1.0f, 2.0f, 3.0f); b.Forward = TVector(4.0f, 5.0f, 6.0f);
    o.SetPositionMatrix(TVector(-1.0f, 0.25f, 8.0f), b, 0.5f);
    const float e2[12] = { -1.5f, 3.0f, -1.5f,  -2.0f, -2.5f, -3.0f,  0.5f, 1.0f, 1.5f,  -1.0f, 0.25f, 8.0f };
    ok = ok && Same(o.PositionMatrix, e2) && o.Scale == 0.5f && o.OO_Scale == 2.0f;
    // 3. the position may be the matrix's own translation row: it is read after the identity reset, so the result is 0 + 0.
    o.SetPositionMatrix(*reinterpret_cast<const TVector*>(&o.PositionMatrix.E41), a, 1.0f);
    const float e3[12] = { -1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f };
    ok = ok && Same(o.PositionMatrix, e3) && o.Scale == 1.0f && o.OO_Scale == 1.0f;
    // 4. the axes are read before the reset: an orientation stored inside the matrix (rows 2 and 3 as Up / Forward) still counts.
    o.PositionMatrix.E21 = 0.0f; o.PositionMatrix.E22 = 0.0f; o.PositionMatrix.E23 = 1.0f;     // Up
    o.PositionMatrix.E31 = 0.0f; o.PositionMatrix.E32 = 1.0f; o.PositionMatrix.E33 = 0.0f;     // Forward
    o.SetPositionMatrix(TVector(7.0f, 8.0f, 9.0f), *reinterpret_cast<const TRightHandedSet*>(&o.PositionMatrix.E21), 4.0f);
    const float e4[12] = { -4.0f, 0.0f, 0.0f,  0.0f, -4.0f, 0.0f,  0.0f, 0.0f, 4.0f,  7.0f, 8.0f, 9.0f };
    ok = ok && Same(o.PositionMatrix, e4) && o.Scale == 4.0f && o.OO_Scale == 0.25f;
    if (!ok) { std::printf("00a114b0_TEST FAIL\n"); return 1; }
    std::printf("00a114b0_TEST PASS\n");
    return 0;
}