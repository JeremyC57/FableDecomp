#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CLine { float m_startX, m_startY, m_startZ, m_endX, m_endY, m_endZ; C3DVector GetWorldPosition() const { C3DVector d(m_endX - m_startX, m_endY - m_startY, m_endZ - m_startZ); return C3DVector(d.X * 0.5f, d.Y * 0.5f, d.Z * 0.5f); } };
int main() { CLine l = { 0, 2, 4, 6, 8, 10 }; C3DVector p = l.GetWorldPosition(); if (p.X != 3 || p.Y != 3 || p.Z != 3) { std::printf("00a11430_TEST FAIL\n"); return 1; } std::printf("00a11430_TEST PASS\n"); return 0; }