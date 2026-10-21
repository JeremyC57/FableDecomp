// Behaviour test for CLinearSpline::GetPointWithParam @ 0x00A57F70 (copy of the model; GetPathLength stubbed as the polyline length).
#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
struct CLinearSpline {
    C3DVector* start; C3DVector* finish;
    float GetPathLength() const { float l = 0; for (const C3DVector* p = start + 1; p < finish; ++p) l += (*p - *(p - 1)).GetLength(); return l; }
    C3DVector GetPointWithParam(float t) const
    {
        if (t < 0.0f) t = 0.0f; else if (t > 1.0f) t = 1.0f;
        float target = GetPathLength() * t;
        const C3DVector* it = start;
        t = 0.0f;
        C3DVector prev = *it;
        for (++it; it != finish; ++it) {
            C3DVector delta = *it - prev;
            float length = delta.GetLength();
            if (t + length > target - 1e-5f) return prev + delta * (target - t) * (1.0f / length);
            t += length;
            prev = *it;
        }
        return prev;
    }
};
static bool near(const C3DVector& v, float x, float y, float z) { return std::fabs(v.X - x) < 1e-3f && std::fabs(v.Y - y) < 1e-3f && std::fabs(v.Z - z) < 1e-3f; }
int main()
{
    int bad = 0;
    C3DVector pts[3] = { C3DVector(0, 0, 0), C3DVector(10, 0, 0), C3DVector(10, 10, 0) };
    CLinearSpline s; s.start = pts; s.finish = pts + 3;
    if (!near(s.GetPointWithParam(0.25f), 5, 0, 0)) bad++;
    if (!near(s.GetPointWithParam(0.75f), 10, 5, 0)) bad++;
    if (!near(s.GetPointWithParam(-1.0f), 0, 0, 0)) bad++;
    if (!near(s.GetPointWithParam(2.0f), 10, 10, 0)) bad++;
    if (!near(s.GetPointWithParam(0.5f), 10, 0, 0)) bad++;
    if (bad) { std::printf("00a57f70_TEST FAIL\n"); return 1; }
    std::printf("00a57f70_TEST PASS\n");
    return 0;
}