#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; }; struct CInputManager { unsigned char _pad[0xc]; C3DVector m_a; C3DVector m_b; unsigned char _pad24[4]; long m_c; void SetVectors(const C3DVector& a, const C3DVector& b, long c) { m_a = a; m_b = b; m_c = c; } };

int main() {
    CInputManager m; std::memset(&m, 0, sizeof m); C3DVector a = { 1, 2, 3 }, b = { 4, 5, 6 }; m.SetVectors(a, b, 7); if (m.m_a.X != 1 || m.m_a.Z != 3 || m.m_b.X != 4 || m.m_b.Z != 6 || m.m_c != 7) { std::printf("007fd3f0_TEST FAIL\n"); return 1; }
    std::printf("007fd3f0_TEST PASS\n"); return 0;
}
