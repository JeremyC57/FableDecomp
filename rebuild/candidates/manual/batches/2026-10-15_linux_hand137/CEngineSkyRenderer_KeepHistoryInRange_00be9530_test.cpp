// Behaviour test for 0x00BE9530 CEngineSkyRenderer::KeepHistoryInRange: both history entries wrap by the range per axis.
#include <cstdio>

struct C2DVector { float X, Y; };
struct CEngineSkyRenderer {
    void KeepHistoryInRange(C2DVector* history, float range)
    {
        if (history[0].X > range) { history[0].X -= range; history[1].X -= range; }
        else if (-range > history[0].X) { history[0].X += range; history[1].X += range; }
        if (history[0].Y > range) { history[0].Y -= range; history[1].Y -= range; }
        else if (-range > history[0].Y) { history[0].Y += range; history[1].Y += range; }
    }
};

int main()
{
    CEngineSkyRenderer sky;
    C2DVector h[2];
    h[0].X = 12.0f; h[0].Y = -11.0f; h[1].X = 11.0f; h[1].Y = -10.0f;
    sky.KeepHistoryInRange(h, 10.0f);
    if (h[0].X != 2.0f || h[1].X != 1.0f || h[0].Y != -1.0f || h[1].Y != 0.0f) { std::printf("00be9530_TEST FAIL\n"); return 1; }
    h[0].X = 10.0f; h[0].Y = -10.0f; h[1].X = 3.0f; h[1].Y = 4.0f;   // boundaries stay
    sky.KeepHistoryInRange(h, 10.0f);
    if (h[0].X != 10.0f || h[1].X != 3.0f || h[0].Y != -10.0f || h[1].Y != 4.0f) { std::printf("00be9530_TEST FAIL\n"); return 1; }
    std::printf("00be9530_TEST PASS\n");
    return 0;
}
