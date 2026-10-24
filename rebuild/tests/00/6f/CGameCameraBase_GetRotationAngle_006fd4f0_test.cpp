// Behaviour test for 006fd4f0 (CGameCameraBase::GetRotationAngle): signed turns from `from` to the XY projection of
// `to` -- a quarter turn either way, zero for parallel vectors, and zero when the cosine is not a number.
#include <cstdio>
#include <cmath>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};

class CGameCameraBase {
public:
    float GetRotationAngle(const C3DVector& from, const C3DVector& to);
};

float CGameCameraBase::GetRotationAngle(const C3DVector& from, const C3DVector& to)
{
    float result = 0.0f;
    C3DVector flatTo(to.X, to.Y, 0.0f);
    float cosAngle = flatTo.Dot(from) / (from.GetLength() * flatTo.GetLength());
    if (cosAngle <= 1.0f && cosAngle >= -1.0f) {
        float angle = (float)std::acos(cosAngle) * 0.15915494f;
        float sign = 1.0f;
        if (flatTo.X * from.Y - flatTo.Y * from.X < 0.0f)
            sign = -1.0f;
        result = angle * sign;
    }
    return result;
}

static int Near(float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; }

int main()
{
    CGameCameraBase camera;
    int ok = 1;
    if (!Near(camera.GetRotationAngle(C3DVector(1, 0, 0), C3DVector(0, 1, 5)), -0.25f))
        ok = 0;
    if (!Near(camera.GetRotationAngle(C3DVector(0, 1, 0), C3DVector(1, 0, -3)), 0.25f))
        ok = 0;
    if (!Near(camera.GetRotationAngle(C3DVector(2, 0, 0), C3DVector(3, 0, 9)), 0.0f))
        ok = 0;
    if (!Near(camera.GetRotationAngle(C3DVector(1, 0, 0), C3DVector(-1, 0, 1)), 0.5f))
        ok = 0;
    if (!Near(camera.GetRotationAngle(C3DVector(1, 1, 0), C3DVector(1, 0, 0)), 0.125f))
        ok = 0;
    if (camera.GetRotationAngle(C3DVector(0, 0, 0), C3DVector(1, 0, 0)) != 0.0f)   // 0/0: the range test fails
        ok = 0;
    if (!ok) {
        std::printf("006fd4f0_TEST FAIL\n");
        return 1;
    }
    std::printf("006fd4f0_TEST PASS\n");
    return 0;
}