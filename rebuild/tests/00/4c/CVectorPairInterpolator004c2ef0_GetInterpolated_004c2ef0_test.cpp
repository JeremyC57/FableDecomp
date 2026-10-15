#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }

};
struct CVectorPairInterpolator_004c2ef0 {
    long m_header; C3DVector m_from, m_to;
    C3DVector GetInterpolated(float t) { return (m_to - m_from) * t + m_from; }
};
int main()
{
    CVectorPairInterpolator_004c2ef0 p; p.m_header = 0; p.m_from = C3DVector(1.0f, -3.0f, 4.0f); p.m_to = C3DVector(3.0f, 1.0f, 6.0f);
    C3DVector r = p.GetInterpolated(0.5f);
    if (r.X != 2.0f || r.Y != -1.0f || r.Z != 5.0f) { std::printf("004c2ef0_TEST FAIL\n"); return 1; }
    std::printf("004c2ef0_TEST PASS\n");
    return 0;
}