// Behaviour test for 00c20340: CSoundPosition::GetDistance -- own length when listener-relative, else |position - listener|
#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    float GetMagnitude() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
static C3DVector Sub(const C3DVector& a, const C3DVector& b) { C3DVector r; r.X = a.X - b.X; r.Y = a.Y - b.Y; r.Z = a.Z - b.Z; return r; }
struct CSoundPosition {
    C3DVector m_position; bool m_bListenerRelative;
    float GetDistance(const C3DVector& listener) const
    {
        if (m_bListenerRelative) return m_position.GetMagnitude();
        return Sub(m_position, listener).GetMagnitude();
    }
};
int main()
{
    CSoundPosition p; p.m_position.X = 3.0f; p.m_position.Y = 4.0f; p.m_position.Z = 0.0f;
    C3DVector l; l.X = 3.0f; l.Y = 0.0f; l.Z = 0.0f;
    p.m_bListenerRelative = true; float a = p.GetDistance(l);
    p.m_bListenerRelative = false; float b = p.GetDistance(l);
    if (std::fabs(a - 5.0f) > 1e-4f || std::fabs(b - 4.0f) > 1e-4f || sizeof(CSoundPosition) != 0x10)
    { std::printf("00c20340_TEST FAIL\n"); return 1; }
    std::printf("00c20340_TEST PASS\n");
    return 0;
}
