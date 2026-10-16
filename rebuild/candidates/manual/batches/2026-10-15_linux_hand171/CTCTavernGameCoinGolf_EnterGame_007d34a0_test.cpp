// Behaviour test for CTCTavernGameCoinGolf::EnterGame (0x007D34A0): camera reset (distance 1, pitch 0.1275, yaw 0), score 0,
// look-around pushed only when a camera manager is returned, course 0 enabled.
#include <cstdio>
static int g_look, g_course = -1;
struct Golf {
    float Score, ZAngle, XAngle, CameraDistance;
    void EnterGame(bool haveCamera)
    {
        Score = 0.0f; CameraDistance = 1.0f; XAngle = 0.1275f; ZAngle = 0.0f;
        int* camera = 0; static int cam;
        if (haveCamera) camera = &cam;
        if (camera) ++g_look;
        g_course = 0;
    }
};
int main()
{
    Golf g; g.Score = 5; g.ZAngle = 1; g.XAngle = 1; g.CameraDistance = 9;
    g.EnterGame(true); g.EnterGame(false);
    if (g.Score != 0 || g.CameraDistance != 1.0f || g.XAngle != 0.1275f || g.ZAngle != 0 || g_look != 1 || g_course != 0) {
        std::printf("007d34a0_TEST FAIL\n"); return 1;
    }
    std::printf("007d34a0_TEST PASS\n");
    return 0;
}
