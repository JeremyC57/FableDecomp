// Behaviour test for CGameCameraManager::ResetRightStickLookAroundVector (0x006976B0): both look-around angle pairs zeroed.
#include <cstdio>
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } };
struct CGameCameraManager {
    C2DVector RightStickLookAroundAngles; C2DVector RightStickTargetLookAroundAngles;
    CGameCameraManager() : RightStickLookAroundAngles(1.0f, 2.0f), RightStickTargetLookAroundAngles(3.0f, 4.0f) { }
    void ResetRightStickLookAroundVector() { RightStickLookAroundAngles = RightStickTargetLookAroundAngles = C2DVector(0.0f, 0.0f); }
};
int main()
{
    CGameCameraManager m; m.ResetRightStickLookAroundVector();
    bool ok = m.RightStickLookAroundAngles.X == 0.0f && m.RightStickLookAroundAngles.Y == 0.0f
           && m.RightStickTargetLookAroundAngles.X == 0.0f && m.RightStickTargetLookAroundAngles.Y == 0.0f;
    if (!ok) { std::printf("006976b0_TEST FAIL\n"); return 1; }
    std::printf("006976b0_TEST PASS\n");
    return 0;
}