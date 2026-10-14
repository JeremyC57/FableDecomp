#include <cstdio>
#include <cstdlib>
#include <cstring>
static const float g_zero_0122dedc = 0.0f; struct C3DVector { float X, Y, Z; };
struct CGetBestIciclePointForTarget { unsigned char _pad[0xc]; float m_bestScore; C3DVector m_bestPosition; bool GetBestPosition(C3DVector* out) { if (m_bestScore > g_zero_0122dedc) { *out = m_bestPosition; return true; } return false; } };

int main() {
    CGetBestIciclePointForTarget g; g.m_bestScore = 0.5f; g.m_bestPosition.X = 1; g.m_bestPosition.Y = 2; g.m_bestPosition.Z = 3; C3DVector out = { 9, 9, 9 };
    if (!g.GetBestPosition(&out) || out.X != 1 || out.Y != 2 || out.Z != 3) { std::printf("008e2b60_TEST FAIL\n"); return 1; } g.m_bestScore = 0.0f; out.X = 9; if (g.GetBestPosition(&out) || out.X != 9) { std::printf("008e2b60_TEST FAIL\n"); return 1; } g.m_bestScore = -1.0f; if (g.GetBestPosition(&out)) { std::printf("008e2b60_TEST FAIL\n"); return 1; }
    std::printf("008e2b60_TEST PASS\n"); return 0;
}