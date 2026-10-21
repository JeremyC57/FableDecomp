// Behaviour test for CTCKraken::DoHeadTracking (0x007FAF60): the facing turns toward the target angle by at most
// speed/360 per call, wrapped into [0, 1); nothing happens when tracking is off or the target is dead.
#include <cstdio>
#include <cmath>

static float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)std::fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
static float AngleDifference(float a, float b)      // shortest signed turn from a to b
{
    float d = b - a;
    if (d > 0.5f) d -= 1.0f;
    if (d < -0.5f) d += 1.0f;
    return d;
}
static const float& Clamp(const float& v, const float& lo, const float& hi) { return v < lo ? lo : (v > hi ? hi : v); }

struct Kraken { bool track, targetAlive, beamAlive; float beamSpeed, targetSpeed, ox, oy, tx, ty, facing; };
static void DoHeadTracking(Kraken& k)
{
    if (k.track && k.targetAlive) {
        float speed = k.beamAlive ? k.beamSpeed : k.targetSpeed;
        float angle = WrapAngle((float)std::atan2(k.tx - k.ox, k.ty - k.oy) * 0.15915494f);
        float facing = k.facing;
        float lo = -(speed * (1.0f / 360.0f)), hi = speed * (1.0f / 360.0f);
        k.facing = WrapAngle(facing + Clamp(AngleDifference(facing, angle), lo, hi));
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Kraken k; k.track = true; k.targetAlive = true; k.beamAlive = false; k.beamSpeed = 3.6f; k.targetSpeed = 36.0f;
    k.ox = 0; k.oy = 0; k.tx = 1; k.ty = 0; k.facing = 0.0f;        // target at angle 0.25
    DoHeadTracking(k);
    if (!Near(k.facing, 0.1f)) { std::printf("007faf60_TEST FAIL\n"); return 1; }
    k.beamAlive = true; DoHeadTracking(k);                          // beam speed 3.6 -> step 0.01
    if (!Near(k.facing, 0.11f)) { std::printf("007faf60_TEST FAIL\n"); return 1; }
    k.facing = 0.98f; k.beamAlive = false; k.tx = 0; k.ty = 1;      // target at 0: small step across the wrap
    DoHeadTracking(k);
    if (!Near(k.facing, 0.0f) && !Near(k.facing, 1.0f)) { std::printf("007faf60_TEST FAIL\n"); return 1; }
    k.facing = 0.5f; k.targetAlive = false; DoHeadTracking(k);
    if (!Near(k.facing, 0.5f)) { std::printf("007faf60_TEST FAIL\n"); return 1; }
    k.targetAlive = true; k.track = false; DoHeadTracking(k);
    if (!Near(k.facing, 0.5f)) { std::printf("007faf60_TEST FAIL\n"); return 1; }
    std::printf("007faf60_TEST PASS\n");
    return 0;
}
