// Behaviour test for 0x00A57EF0 NBBBMaths::CLinearSpline::GetLength() const
#include <cstdio>
#include <cmath>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float GetLength() const { return (float)std::sqrt(GetLengthSquared()); }
};

namespace NBBBMaths {
struct C3DVectorArray {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
};
class CLinearSpline {
public:
    C3DVectorArray ControlPoints;
    C3DVectorArray ProcessedPoints;
    float GetLength() const
    {
        float length = 0.0f;
        const C3DVector* it = ProcessedPoints._M_start;
        C3DVector prev = *it;
        for (++it; it != ProcessedPoints._M_finish; ++it) {
            length += (*it - prev).GetLength();
            prev = *it;
        }
        return length;
    }
};
}   // namespace NBBBMaths

int main()
{
    C3DVector pts[4] = { C3DVector(0.0f, 0.0f, 0.0f), C3DVector(3.0f, 4.0f, 0.0f), C3DVector(3.0f, 4.0f, 12.0f),
                         C3DVector(3.0f, 4.0f, 12.0f) };
    NBBBMaths::CLinearSpline s;
    s.ControlPoints._M_start = s.ControlPoints._M_finish = s.ControlPoints._M_end_of_storage = 0;
    s.ProcessedPoints._M_start = pts;
    s.ProcessedPoints._M_finish = pts + 4;              // 5 + 12 + a zero-length last segment
    s.ProcessedPoints._M_end_of_storage = pts + 4;
    float len = s.GetLength();
    s.ProcessedPoints._M_finish = pts + 1;              // a single point has no length
    float one = s.GetLength();
    if (std::fabs(len - 17.0f) > 0.001f || std::fabs(one) > 0.001f) { std::printf("00a57ef0_TEST FAIL\n"); return 1; }
    std::printf("00a57ef0_TEST PASS\n");
    return 0;
}