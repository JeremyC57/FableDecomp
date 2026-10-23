// Behaviour test for CTCPhysicsBase::GetPositionInterpolator (0x006B0250): without bullet time the pair is
// (OldPosition, Position); under bullet time it is the two lerps at sub-steps (frame-1)%scale and +1, step size 1/scale.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
static V Lerp(const V& a, const V& b, float t)
{
    V r; r.X = (b.X - a.X) * t + a.X; r.Y = (b.Y - a.Y) * t + a.Y; r.Z = (b.Z - a.Z) * t + a.Z; return r;
}
struct Interp { V start, end; };

static bool g_active, g_affected;
static float g_scale;
static int g_frame;

static void GetPositionInterpolator(const V& pos, const V& old, Interp& out)
{
    if (g_active && g_affected) {
        float timeScale = g_scale;
        int step = (g_frame - 1) % (int)timeScale;
        float stepSize = 1.0f / timeScale;
        V s = Lerp(old, pos, step * stepSize);
        ++step;
        V e = Lerp(old, pos, step * stepSize);
        out.start = s; out.end = e;
    } else {
        out.start = old; out.end = pos;
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    V pos = { 10.0f, 20.0f, 30.0f };
    V old = { 0.0f, 0.0f, 0.0f };
    Interp r;
    g_active = false; g_affected = true;
    GetPositionInterpolator(pos, old, r);
    if (!Near(r.start.X, 0.0f) || !Near(r.end.Z, 30.0f)) { std::printf("006b0250_TEST FAIL\n"); return 1; }
    g_active = true; g_scale = 4.0f; g_frame = 7;   // step 2 of 4 -> 0.5 and 0.75
    GetPositionInterpolator(pos, old, r);
    if (!Near(r.start.X, 5.0f) || !Near(r.start.Z, 15.0f) || !Near(r.end.Y, 15.0f)) { std::printf("006b0250_TEST FAIL\n"); return 1; }
    g_affected = false;
    GetPositionInterpolator(pos, old, r);
    if (!Near(r.start.Y, 0.0f) || !Near(r.end.X, 10.0f)) { std::printf("006b0250_TEST FAIL\n"); return 1; }
    std::printf("006b0250_TEST PASS\n");
    return 0;
}
