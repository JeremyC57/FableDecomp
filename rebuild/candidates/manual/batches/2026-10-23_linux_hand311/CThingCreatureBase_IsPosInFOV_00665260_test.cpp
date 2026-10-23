// Behaviour test for CThingCreatureBase::IsPosInFOV (0x00665260): wrapped XY angle to the position within half the field of view of the facing (plus the look offset when asked).
#include <cstdio>
#include <cmath>
static float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)std::fmod((double)angle, 1.0);
        if (angle < 0.0f) angle += 1.0f;
    }
    return angle;
}
static float AngleDistance(float a, float b)
{
    a = WrapAngle(a);
    float d = (float)std::fabs(a - WrapAngle(b));
    if (d > 0.5f) d = 1.0f - d;
    return d;
}
struct Creature {
    bool hasFov, hasLook; float fov, x, y, dirX, dirY, lookOffset;
    bool IsPosInFOV(float px, float py, bool useLook) const
    {
        if (!hasFov) return false;
        float dx = px - x, dy = py - y;
        float angle = WrapAngle((float)std::atan2(dirX, dirY) * 0.15915494f);
        float facing = angle;
        if (hasLook && useLook) facing = WrapAngle(lookOffset + angle);
        float target = WrapAngle((float)std::atan2(dx, dy) * 0.15915494f);
        return AngleDistance(target, facing) <= fov * 0.5f;
    }
};
int main()
{
    Creature c; c.hasFov = true; c.hasLook = true; c.fov = 0.25f; c.x = 0.0f; c.y = 0.0f; c.dirX = 0.0f; c.dirY = 1.0f; c.lookOffset = 0.25f;
    if (!c.IsPosInFOV(0.1f, 5.0f, false)) { std::printf("ahead 00665260_TEST FAIL\n"); return 1; }
    if (c.IsPosInFOV(0.0f, -5.0f, false)) { std::printf("behind 00665260_TEST FAIL\n"); return 1; }
    if (c.IsPosInFOV(5.0f, 0.0f, false)) { std::printf("side 00665260_TEST FAIL\n"); return 1; }
    if (!c.IsPosInFOV(5.0f, 0.0f, true)) { std::printf("look 00665260_TEST FAIL\n"); return 1; }
    if (!c.IsPosInFOV(-0.2f, 5.0f, false)) { std::printf("wrap 00665260_TEST FAIL\n"); return 1; }
    c.hasFov = false;
    if (c.IsPosInFOV(0.1f, 5.0f, false)) { std::printf("nofov 00665260_TEST FAIL\n"); return 1; }
    std::printf("00665260_TEST PASS\n");
    return 0;
}
