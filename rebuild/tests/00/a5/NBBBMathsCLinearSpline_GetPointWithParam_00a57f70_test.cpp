// Link test for 0x00A57F70: the source object's NBBBMaths::CLinearSpline::GetPointWithParam over a processed polyline,
// with GetLength (0x00A57EF0) supplied here as the polyline length.
#include <cstdio>
#include <cmath>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
struct C3DVectorArray {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
};
namespace NBBBMaths {
class CLinearSpline {
public:
    C3DVectorArray ControlPoints;                  // +0x00
    C3DVectorArray ProcessedPoints;                // +0x0c
    float GetLength() const;                       // 0x00A57EF0
    C3DVector GetPointWithParam(float t) const;   // from the source object
};
}

static int g_lengthCalls = 0;
float NBBBMaths::CLinearSpline::GetLength() const
{
    ++g_lengthCalls;
    float l = 0;
    for (const C3DVector* p = ProcessedPoints._M_start + 1; p < ProcessedPoints._M_finish; ++p)
        l += (*p - *(p - 1)).GetLength();
    return l;
}
static bool near(const C3DVector& v, float x, float y, float z) { return std::fabs(v.X - x) < 1e-3f && std::fabs(v.Y - y) < 1e-3f && std::fabs(v.Z - z) < 1e-3f; }
int main()
{
    int bad = 0;
    C3DVector pts[3] = { C3DVector(0, 0, 0), C3DVector(10, 0, 0), C3DVector(10, 10, 0) };
    NBBBMaths::CLinearSpline s;
    s.ControlPoints._M_start = s.ControlPoints._M_finish = s.ControlPoints._M_end_of_storage = 0;
    s.ProcessedPoints._M_start = pts; s.ProcessedPoints._M_finish = pts + 3; s.ProcessedPoints._M_end_of_storage = pts + 3;
    if (!near(s.GetPointWithParam(0.25f), 5, 0, 0)) bad |= 1;
    if (!near(s.GetPointWithParam(0.75f), 10, 5, 0)) bad |= 2;
    if (!near(s.GetPointWithParam(-1.0f), 0, 0, 0)) bad |= 4;
    if (!near(s.GetPointWithParam(2.0f), 10, 10, 0)) bad |= 8;
    if (!near(s.GetPointWithParam(0.5f), 10, 0, 0)) bad |= 16;
    if (g_lengthCalls != 5) bad |= 32;
    s.ProcessedPoints._M_finish = pts + 1;
    if (!near(s.GetPointWithParam(0.5f), 0, 0, 0)) bad |= 64;
    if (bad) { std::printf("00a57f70_TEST FAIL %d\n", bad); return 1; }
    std::printf("00a57f70_TEST PASS\n");
    return 0;
}
