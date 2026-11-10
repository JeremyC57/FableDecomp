// Behaviour test for CTCWeatherVane::FrameUpdate (0x00732D40): with a physics component, the vane faces the normalised wind
// direction: angle = WrapAngle(atan2(x, y) / 2pi) less the vane's initial offset; without one nothing happens.
#include <stdio.h>
#include <math.h>

static float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
struct Physics { float facing; int calls; };
static void FrameUpdate(Physics* physics, float wx, float wy, float wz, float initialOffset)
{
    if (physics) {
        float m = (float)sqrt(wx * wx + wy * wy + wz * wz);
        float dx = wx / m, dy = wy / m;
        float angle = WrapAngle((float)atan2(dx, dy) * 0.15915494f);
        physics->facing = angle - initialOffset;
        ++physics->calls;
    }
}
static bool Near(float a, float b) { return fabs(a - b) < 0.0005f; }
int main()
{
    int fails = 0;
    Physics p = { -9.0f, 0 };
    FrameUpdate(&p, 0.0f, 3.0f, 0.0f, 0.0f);
    if (!Near(p.facing, 0.0f)) { printf("north FAIL %f\n", p.facing); ++fails; }
    FrameUpdate(&p, 2.0f, 0.0f, 1.0f, 0.0f);
    if (!Near(p.facing, 0.25f)) { printf("east FAIL %f\n", p.facing); ++fails; }
    FrameUpdate(&p, -1.0f, 0.0f, 0.0f, 0.1f);
    if (!Near(p.facing, 0.65f)) { printf("west wrap FAIL %f\n", p.facing); ++fails; }
    if (p.calls != 3) { printf("calls FAIL\n"); ++fails; }
    FrameUpdate(0, 1.0f, 1.0f, 0.0f, 0.0f);
    if (!Near(WrapAngle(1.25f), 0.25f) || !Near(WrapAngle(-0.25f), 0.75f) || !Near(WrapAngle(0.5f), 0.5f)) { printf("wrap FAIL\n"); ++fails; }
    if (fails) { printf("00732d40_TEST FAIL\n"); return 1; }
    printf("00732d40_TEST PASS\n");
    return 0;
}