#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CLine {
    C3DVector m_start, m_end;
    bool GetWorldPosition(C3DVector& position) { position = (m_end - m_start) * 0.5f + m_start; return true; }
};
int main()
{
    CLine l; l.m_start = C3DVector(1.0f, 2.0f, -4.0f); l.m_end = C3DVector(3.0f, 6.0f, 4.0f);
    C3DVector p(0.0f, 0.0f, 0.0f);
    if (!l.GetWorldPosition(p) || p.X != 2.0f || p.Y != 4.0f || p.Z != 0.0f) { std::printf("00bc7d40_TEST FAIL\n"); return 1; }
    std::printf("00bc7d40_TEST PASS\n");
    return 0;
}