#include <cstdio>
#include <cmath>
// Behaviour test for CEngineLandscapeRenderer::PatchWithinFadeStartSphere(long, long) @ 0x00B670E0: true when the 3D
// distance from (x, y, 0) to the camera position is strictly below the foreground fade start.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Length() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
static C3DVector g_cameraPos(0.0f, 0.0f, 0.0f);
inline const C3DVector& PeekCameraPos() { return g_cameraPos; }
class CEngineLandscapeRenderer {
public:
    float m_foregroundFadeStart;
    float m_foregroundFadeEnd;
    bool PatchWithinFadeStartSphere(long x, long y)
    {
        C3DVector offset = C3DVector((float)x, (float)y, 0.0f) - PeekCameraPos();
        if (offset.Length() < m_foregroundFadeStart)
            return true;
        return false;
    }
};
int main()
{
    CEngineLandscapeRenderer r;
    r.m_foregroundFadeStart = 5.0f; r.m_foregroundFadeEnd = 9.0f;
    g_cameraPos = C3DVector(10.0f, 20.0f, 0.0f);
    if (!r.PatchWithinFadeStartSphere(12, 22)) { std::printf("00b670e0_TEST FAIL\n"); return 1; }
    if (r.PatchWithinFadeStartSphere(13, 24)) { std::printf("00b670e0_TEST FAIL\n"); return 1; }    // exactly 5: not below
    if (r.PatchWithinFadeStartSphere(16, 20)) { std::printf("00b670e0_TEST FAIL\n"); return 1; }
    g_cameraPos = C3DVector(10.0f, 20.0f, 4.0f);                                                        // height counts
    if (r.PatchWithinFadeStartSphere(13, 20)) { std::printf("00b670e0_TEST FAIL\n"); return 1; }
    if (!r.PatchWithinFadeStartSphere(12, 21)) { std::printf("00b670e0_TEST FAIL\n"); return 1; }
    g_cameraPos = C3DVector(-3.0f, -4.0f, 0.0f);
    if (!r.PatchWithinFadeStartSphere(-1, -2)) { std::printf("00b670e0_TEST FAIL\n"); return 1; }
    std::printf("00b670e0_TEST PASS\n");
    return 0;
}