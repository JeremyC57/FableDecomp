// Behaviour test for CGameCameraManager::SetupCameraMode (0x0069E2E0), g++ model: a string-and-cage mode from the first available camera point's parameters (FOV / 360).
#include <cstdio>
#include <vector>
struct Point { float length, cage, height, fov, autoTime; bool autoGoBehind; };
struct Mode { float a, b, c, d, angle; bool flag; };
static Mode SetupCameraMode(const std::vector<Point>& points)
{
    const Point& p = points.front();
    Mode m = { p.length, p.cage, p.height, p.autoTime, p.fov * (1.0f / 360.0f), p.autoGoBehind };
    return m;
}
int main()
{
    std::vector<Point> pts;
    Point p = { 8.0f, 2.5f, 1.25f, 90.0f, 0.75f, true }, q = { 1, 1, 1, 1, 1, false };
    pts.push_back(p); pts.push_back(q);
    Mode m = SetupCameraMode(pts);
    int ok = m.a == 8.0f && m.b == 2.5f && m.c == 1.25f && m.d == 0.75f && m.flag && m.angle == 90.0f * (1.0f / 360.0f);
    std::printf(ok ? "0069e2e0_TEST PASS\n" : "0069e2e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
