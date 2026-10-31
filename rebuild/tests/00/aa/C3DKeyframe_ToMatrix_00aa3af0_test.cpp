// Behaviour test for C3DKeyframe::ToMatrix(CMatrix4x4&, bool) 0x00AA3AF0: the keyframe quaternion becomes the
// transposed rotation block, the position the fourth column, and the last row is 0,0,0,1.
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class C4DVector { public: float X, Y, Z, W; };
class CMatrix4x4 { public:
    union {
        struct { float E11, E12, E13, E14, E21, E22, E23, E24, E31, E32, E33, E34, E41, E42, E43, E44; } s;
        float M[4][4];
    };
    float& operator()(int row, int col) { return M[row][col]; }
};
class CQuaternion { public:
    float X, Y, Z, W;
    void ToMatrix(CMatrix4x4& m, const C3DVector& pos) const
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
    void ToMatrix(CMatrix4x4& m, bool applyScaling) const
    {
        Rotation.ToMatrix(m, C3DVector(Position.X, Position.Y, Position.Z));
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    C3DKeyframe k;
    float h = (float)std::sqrt(0.5);
    k.Rotation.X = 0.0f; k.Rotation.Y = 0.0f; k.Rotation.Z = h; k.Rotation.W = h;   // 90 degrees about Z
    k.Position.X = 1.0f; k.Position.Y = 2.0f; k.Position.Z = 3.0f; k.Position.W = 9.0f;
    k.Scaling.X = k.Scaling.Y = k.Scaling.Z = k.Scaling.W = 5.0f;
    CMatrix4x4 m;
    for (int i = 0; i < 16; ++i) m.M[i / 4][i % 4] = -7.0f;
    k.ToMatrix(m, true);
    float want[16] = { 0, 1, 0, 1,  -1, 0, 0, 2,  0, 0, 1, 3,  0, 0, 0, 1 };
    for (int i = 0; i < 16; ++i)
        if (!Near(m.M[i / 4][i % 4], want[i])) { std::printf("00aa3af0_TEST FAIL\n"); return 1; }
    // the zero quaternion takes the s = 1 branch: identity-free 1 - 0 diagonal
    k.Rotation.Z = 0.0f; k.Rotation.W = 0.0f;
    k.ToMatrix(m, false);
    if (!Near(m.s.E11, 1.0f) || !Near(m.s.E22, 1.0f) || !Near(m.s.E33, 1.0f) || !Near(m.s.E12, 0.0f) || !Near(m.s.E44, 1.0f)) {
        std::printf("00aa3af0_TEST FAIL\n"); return 1;
    }
    std::printf("00aa3af0_TEST PASS\n");
    return 0;
}