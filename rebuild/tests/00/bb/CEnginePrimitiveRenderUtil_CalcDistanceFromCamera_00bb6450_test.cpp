// behaviour test for 0x00BB6450 CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const CMatrix3x4&)
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; Vec(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct Matrix { float E[9]; Vec Pos; Matrix() : Pos(0.0f, 0.0f, 0.0f) { for (int i = 0; i < 9; ++i) E[i] = 0.0f; } };
struct Camera { Vec WorldPosition; Camera() : WorldPosition(0.0f, 0.0f, 0.0f) { } };
static Camera gCamera;

static float CalcDistanceFromCamera(const Matrix& m)
{
    Vec p(m.Pos.X, m.Pos.Y, m.Pos.Z);
    Vec d(p.X - gCamera.WorldPosition.X, p.Y - gCamera.WorldPosition.Y, p.Z - gCamera.WorldPosition.Z);
    return (float)std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }

int main()
{
    Matrix m;
    m.E[0] = 9.0f;                 // the rotation part is ignored
    m.Pos = Vec(4.0f, 6.0f, 3.0f);
    gCamera.WorldPosition = Vec(1.0f, 2.0f, 3.0f);
    if (!Near(CalcDistanceFromCamera(m), 5.0f)) { std::printf("00bb6450_TEST FAIL\n"); return 1; }
    gCamera.WorldPosition = Vec(4.0f, 6.0f, 3.0f);
    if (!Near(CalcDistanceFromCamera(m), 0.0f)) { std::printf("00bb6450_TEST FAIL\n"); return 1; }
    gCamera.WorldPosition = Vec(4.0f, 6.0f, -9.0f);
    if (!Near(CalcDistanceFromCamera(m), 12.0f)) { std::printf("00bb6450_TEST FAIL\n"); return 1; }
    std::printf("00bb6450_TEST PASS\n");
    return 0;
}