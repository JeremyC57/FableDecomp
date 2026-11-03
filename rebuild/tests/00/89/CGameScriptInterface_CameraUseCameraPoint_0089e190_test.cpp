#include <cstdio>
// Model of CGameScriptInterface::CameraUseCameraPoint: a live camera point picks its scripted mode -- type 6 (scripted) builds a
// transition between its start/end positions (looking along start+look / end+look, or at the thing with ThingUID), with the
// point's transition time in world frames and its FOV (or the caller's, when in (0, 180)) as a fraction of a turn; type 7
// (spline) a spline between the two keys. The mode is anchored at pos / axes, bound to the point and handed to the manager.
struct Vec { float X, Y, Z; };
static Vec Add(const Vec& a, const Vec& b) { Vec r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
struct Point { int type; bool alive; Vec start, end, look, lookEnd; unsigned int uid; float fov, time; };
struct Mode { int kind; Vec lookFrom, lookTo; long frames; float fov; long a, b; bool selfTerminate; bool hasOrigin; Vec origin; bool hasAxes; };
static const long ConstantFPS = 10;
static bool Use(const Point& p, const Vec* pos, bool axes, float fov, long a, long b, Mode& out)
{
    if (!p.alive) return false;
    Mode m = { 0 };
    if (p.type == 6) {
        long frames = (long)(ConstantFPS * p.time);
        float f = p.fov * (1.0f / 360.0f);
        if (fov > 0.0f && fov < 180.0f) f = fov * (1.0f / 360.0f);
        if (p.uid == 0) { m.kind = 1; m.lookFrom = Add(p.start, p.look); m.lookTo = Add(p.end, p.lookEnd); }
        else m.kind = 2;
        m.frames = frames; m.fov = f;
    } else if (p.type == 7) { m.kind = 3; m.a = a; m.b = b; }
    if (!m.kind) return false;
    m.selfTerminate = true;
    if (pos) { m.hasOrigin = true; m.origin = *pos; }
    m.hasAxes = axes;
    out = m;
    return true;
}
int main()
{
    Point p = { 6, true, {1,2,3}, {4,5,6}, {1,1,1}, {2,2,2}, 0, 90.0f, 2.5f };
    Vec origin = { 7, 8, 9 };
    Mode m;
    if (!Use(p, &origin, true, 45.0f, 0, 0, m) || m.kind != 1 || m.frames != 25 || m.lookFrom.X != 2.0f || m.lookTo.Z != 8.0f ||
        m.fov != 45.0f * (1.0f / 360.0f) || !m.selfTerminate || !m.hasOrigin || m.origin.Y != 8.0f) { std::printf("0089e190_TEST FAIL\n"); return 1; }
    if (!Use(p, 0, false, 200.0f, 0, 0, m) || m.fov != 90.0f * (1.0f / 360.0f) || m.hasOrigin) { std::printf("0089e190_TEST FAIL\n"); return 1; }
    p.uid = 5; if (!Use(p, 0, false, 0.0f, 0, 0, m) || m.kind != 2) { std::printf("0089e190_TEST FAIL\n"); return 1; }
    p.type = 7; if (!Use(p, 0, false, 0.0f, 3, 9, m) || m.kind != 3 || m.a != 3 || m.b != 9) { std::printf("0089e190_TEST FAIL\n"); return 1; }
    p.type = 3; if (Use(p, 0, false, 0.0f, 0, 0, m)) { std::printf("0089e190_TEST FAIL\n"); return 1; }
    p.alive = false; p.type = 6; if (Use(p, 0, false, 0.0f, 0, 0, m)) { std::printf("0089e190_TEST FAIL\n"); return 1; }
    std::printf("0089e190_TEST PASS\n");
    return 0;
}