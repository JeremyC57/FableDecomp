// Behaviour test for 00715a70 (NTCScriptedControl::CActionTalkBase::SetUpCamera): a standalone copy of the zoom factor clamp and the
// shoulder camera offsets -- side = (dir.Y, -dir.X, 0) * k, left = from - side, right = from + side, behind = shoulder - dir * k2.
#include <cstdio>
#include <cmath>
static float Zoom(float dist, float nearDist, float scale)
{
    float z = (dist - nearDist) * scale;
    if (z < 0.0f) z = 0.0f; else if (z > 1.0f) z = 1.0f;
    return z;
}
int main()
{
    int fails = 0;
    if (Zoom(0.5f, 1.0f, 0.5f) != 0.0f || Zoom(10.0f, 1.0f, 0.5f) != 1.0f || Zoom(2.0f, 1.0f, 0.5f) != 0.5f) ++fails;
    float dx = 0, dy = 1, fx = 5, fy = 5, k = 0.5f;
    float sx = dy * k, sy = -dx * k;
    float lx = fx - sx, ly = fy - sy, rx = fx + sx, ry = fy + sy;
    if (lx != 4.5f || ly != 5.0f || rx != 5.5f || ry != 5.0f) ++fails;
    float bx = lx - dx * 0.5f, by = ly - dy * 0.5f;
    if (bx != 4.5f || by != 4.5f) ++fails;
    if (fails) { std::printf("00715a70_TEST FAIL %d\n", fails); return 1; }
    std::printf("00715a70_TEST PASS\n");
    return 0;
}
