// Behaviour test for 0x00661540 GFGetAngleXYTo (model copy)
#include <cstdio>
#include <cmath>
struct C3DVector { float X, Y, Z; };
float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)std::fmod((double)angle, 1.0);
        if (angle < 0.0f) angle += 1.0f;
    }
    return angle;
}
float GetAngleXYTo(const C3DVector& from, const C3DVector& to)
{
    float angle = (float)std::atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f;
    return WrapAngle(angle);
}
static bool near_(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    C3DVector o = { 0, 0, 0 }, n = { 0, 1, 0 }, e = { 1, 0, 0 }, s = { 0, -1, 0 }, w = { -1, 0, 5 };
    if (!near_(GetAngleXYTo(o, n), 0.0f) || !near_(GetAngleXYTo(o, e), 0.25f) || !near_(GetAngleXYTo(o, s), 0.5f) || !near_(GetAngleXYTo(o, w), 0.75f))
        { std::printf("00661540_TEST FAIL\n"); return 1; }
    if (!near_(WrapAngle(1.25f), 0.25f) || !near_(WrapAngle(-0.25f), 0.75f) || !near_(WrapAngle(0.5f), 0.5f)) { std::printf("00661540_TEST FAIL\n"); return 1; }
    std::printf("00661540_TEST PASS\n");
    return 0;
}