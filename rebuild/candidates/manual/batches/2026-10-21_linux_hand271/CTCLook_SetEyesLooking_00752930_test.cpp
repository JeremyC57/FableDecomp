// Behaviour test for CTCLook::SetEyesLooking (0x00752930): with a live creature the eye angles are the differences from the
// head facing (body + head angle, wrapped into [0, 1)) to the head->pos direction, or to the recovery angles when pos is
// (within 1e-4) the head position.
#include <cstdio>
#include <cmath>

static float WrapAngle(float a)
{
    if (a < 0.0f || a >= 1.0f) {
        a = (float)std::fmod((double)a, 1.0);
        if (a < 0.0f) a += 1.0f;
    }
    return a;
}
static float AngleDifference(float a, float b) { return b - a; }
static float AngleXY(float hx, float hy, float px, float py) { return WrapAngle((float)std::atan2(px - hx, py - hy) * 0.15915494f); }
struct Look { bool alive; float bodyXY, bodyYZ, headRelXY, headRelYZ, recXY, recYZ, hx, hy, hz, eyeXY, eyeYZ; int changes; };
static void SetEyesLooking(Look& l, float px, float py, float pz)
{
    if (!l.alive) return;
    float headXY = WrapAngle(l.bodyXY + l.headRelXY);
    float headYZ = WrapAngle(l.bodyYZ + l.headRelYZ);
    bool differs = std::fabs(px - l.hx) > 1e-4f || std::fabs(py - l.hy) > 1e-4f || std::fabs(pz - l.hz) > 1e-4f;
    if (differs) {
        l.eyeXY = AngleDifference(headXY, AngleXY(l.hx, l.hy, px, py));
        l.eyeYZ = AngleDifference(headYZ, 0.0f);
    } else {
        l.eyeXY = AngleDifference(headXY, l.recXY);
        l.eyeYZ = AngleDifference(headYZ, l.recYZ);
    }
    ++l.changes;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Look l; l.alive = true; l.bodyXY = 0.9f; l.bodyYZ = 0.0f; l.headRelXY = 0.2f; l.headRelYZ = 0.0f;
    l.recXY = 0.5f; l.recYZ = 0.25f; l.hx = 0; l.hy = 0; l.hz = 0; l.eyeXY = 0; l.eyeYZ = 0; l.changes = 0;
    SetEyesLooking(l, 0.0f, 0.0f, 0.00001f);                   // at the head: recovery angles
    if (!Near(l.eyeXY, 0.4f) || !Near(l.eyeYZ, 0.25f) || l.changes != 1) { std::printf("00752930_TEST FAIL\n"); return 1; }
    SetEyesLooking(l, 1.0f, 0.0f, 0.0f);                       // target at 0.25, head at 0.1
    if (!Near(l.eyeXY, 0.15f)) { std::printf("00752930_TEST FAIL\n"); return 1; }
    l.alive = false; SetEyesLooking(l, 0.0f, 1.0f, 0.0f);
    if (l.changes != 2) { std::printf("00752930_TEST FAIL\n"); return 1; }
    std::printf("00752930_TEST PASS\n");
    return 0;
}
