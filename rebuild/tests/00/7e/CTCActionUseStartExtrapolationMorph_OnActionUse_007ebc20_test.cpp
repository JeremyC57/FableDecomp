#include <cstdio>
#include <cmath>
// Behaviour test: CTCActionUseStartExtrapolationMorph::OnActionUse -- turns the user to face the morph thing: atan2 of the offset as a turn fraction, wrapped into [0,1), plus a half turn.
#define CHECK(c) if (!(c)) { std::printf("007ebc20_TEST FAIL\n"); return 1; }
struct C3DVector { float X, Y, Z; };
static float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)std::fmod((double)angle, 1.0);
        if (angle < 0.0f) angle += 1.0f;
    }
    return angle;
}
static float g_facing = -1.0f;
static int g_baseCalls = 0;
static void OnActionUse(const C3DVector& from, const C3DVector& to)
{
    ++g_baseCalls;
    float angle = (float)std::atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f;
    angle = WrapAngle(angle);
    g_facing = angle + 0.5f;
}
static bool Near(float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; }
int main()
{
    C3DVector o = { 0.0f, 0.0f, 0.0f };
    C3DVector north = { 0.0f, 5.0f, 0.0f }, east = { 5.0f, 0.0f, 0.0f }, west = { -5.0f, 0.0f, 0.0f };
    OnActionUse(o, north); CHECK(Near(g_facing, 0.5f))
    OnActionUse(o, east); CHECK(Near(g_facing, 0.75f))
    OnActionUse(o, west); CHECK(Near(g_facing, 1.25f))
    CHECK(g_baseCalls == 3)
    std::printf("007ebc20_TEST PASS\n");
    return 0;
}