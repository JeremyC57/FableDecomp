#include <cstdio>
#include <cmath>
// Behaviour model of CTCLook::CentreHead (0x007528A0).
struct Look {
    float headXY, headYZ, velXY, velYZ; long state, timeToStop; bool target, oneFrame; int turns;
    void CentreHead()
    {
        target = false;
        if ((float)std::fabs(headXY) < 1e-4f && (float)std::fabs(headYZ) < 1e-4f) {
            if (state != 9) { state = 9; oneFrame = true; }
            timeToStop = -1; velXY = 0.0f; velYZ = 0.0f;
            return;
        }
        turns += 2;
    }
};
int main()
{
    Look a = { 0.00001f, -0.00002f, 3.0f, 4.0f, 2, 50, true, false, 0 };
    a.CentreHead();
    if (a.target || a.state != 9 || !a.oneFrame || a.timeToStop != -1 || a.velXY != 0.0f || a.velYZ != 0.0f || a.turns) { std::printf("007528a0_TEST FAIL\n"); return 1; }
    Look b = { 0.5f, 0.0f, 3.0f, 4.0f, 2, 50, true, false, 0 };
    b.CentreHead();
    if (b.target || b.state != 2 || b.turns != 2 || b.velXY != 3.0f) { std::printf("007528a0_TEST FAIL\n"); return 1; }
    std::printf("007528a0_TEST PASS\n");
    return 0;
}