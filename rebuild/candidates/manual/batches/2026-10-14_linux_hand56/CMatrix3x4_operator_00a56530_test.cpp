#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct CMatrix3x4 { C3DVector m_row0, m_row1, m_row2, m_translation; C3DVector operator*(const C3DVector& v) const { C3DVector r; r.X = m_row0.X * v.X + m_row0.Y * v.Y + m_row0.Z * v.Z; r.Y = m_row1.X * v.X + m_row1.Y * v.Y + m_row1.Z * v.Z; r.Z = m_row2.X * v.X + m_row2.Y * v.Y + m_row2.Z * v.Z; return r; } };

int main() {
    CMatrix3x4 m; float* f = (float*)&m; for (int i = 0; i < 12; ++i) f[i] = (float)(i + 1); C3DVector v; v.X = 1; v.Y = 2; v.Z = 3; C3DVector r = m * v; if (r.X != 14.0f || r.Y != 32.0f || r.Z != 50.0f) { std::printf("00a56530_TEST FAIL\n"); return 1; }
    std::printf("00a56530_TEST PASS\n"); return 0;
}
