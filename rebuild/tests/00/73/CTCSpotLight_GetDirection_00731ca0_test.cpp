#include <cstdio>
#include <cmath>
// Behaviour model of CTCSpotLight::GetDirection (0x00731CA0): flatten the facing onto XY, normalise it, take the horizontal
// perpendicular axis (y, -x, 0) and rotate the direction by 0.25 - angle (overridden info angle or def angle).
struct V { float x, y, z; };
static V Direction(V facing, bool overridden, float infoAngle, float defAngle, V* axisOut, float* rotOut)
{
    V d = facing; d.z = 0.0f;
    float len = std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
    d.x /= len; d.y /= len; d.z /= len;
    V axis = { d.y, -d.x, 0.0f };
    float angle = overridden ? infoAngle : defAngle;
    *axisOut = axis; *rotOut = 0.25f - angle;
    return d;
}
int main()
{
    V f = { 3.0f, 4.0f, 9.0f }, axis; float rot;
    V d = Direction(f, true, 0.1f, 0.2f, &axis, &rot);
    bool ok = std::fabs(d.x - 0.6f) < 1e-5f && std::fabs(d.y - 0.8f) < 1e-5f && d.z == 0.0f
           && std::fabs(axis.x - 0.8f) < 1e-5f && std::fabs(axis.y + 0.6f) < 1e-5f && std::fabs(rot - 0.15f) < 1e-5f;
    Direction(f, false, 0.1f, 0.2f, &axis, &rot);
    ok = ok && std::fabs(rot - 0.05f) < 1e-5f;
    if (!ok) { std::printf("00731ca0_TEST FAIL\n"); return 1; }
    std::printf("00731ca0_TEST PASS\n");
    return 0;
}