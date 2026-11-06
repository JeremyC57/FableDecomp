// Behaviour test for 00bc8a60 (CEnginePrimitiveManagerLines::RenderLine): a standalone copy of the camera-space part -- clip the line
// at the near plane, project both ends, and offset them by the screen-space perpendicular scaled to half the width.
#include <cstdio>
#include <cmath>
struct V3 { float x, y, z; };
static bool Clip(V3& s, V3& e, float nearp)
{
    if (s.z < nearp) {
        if (e.z < nearp) return false;
        V3 d = { e.x - s.x, e.y - s.y, e.z - s.z }; float t = (nearp - s.z) / d.z;
        s.x += d.x * t; s.y += d.y * t; s.z += d.z * t;
    } else if (e.z < nearp) {
        V3 d = { s.x - e.x, s.y - e.y, s.z - e.z }; float t = (nearp - e.z) / d.z;
        e.x += d.x * t; e.y += d.y * t; e.z += d.z * t;
    }
    return true;
}
int main()
{
    int fails = 0;
    V3 s = { 0, 0, -1 }, e = { 0, 0, 3 };
    if (!Clip(s, e, 1.0f) || s.z != 1.0f || e.z != 3.0f) ++fails;
    V3 s2 = { 0, 0, -2 }, e2 = { 0, 0, -1 };
    if (Clip(s2, e2, 1.0f)) ++fails;                                 // both behind the near plane: nothing drawn
    float dx = 3, dy = 4, len = std::sqrt(dx * dx + dy * dy); dx /= len; dy /= len;
    float w = 2.0f * 0.5f; dx *= w; dy *= w;
    float px = dy, py = -dx;                                         // perpendicular (dir.Y, -dir.X)
    if (std::fabs(px - 0.8f) > 1e-6f || std::fabs(py + 0.6f) > 1e-6f) ++fails;
    if (fails) { std::printf("00bc8a60_TEST FAIL %d\n", fails); return 1; }
    std::printf("00bc8a60_TEST PASS\n");
    return 0;
}
