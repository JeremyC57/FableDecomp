// Behaviour test for 0x00752510 CTCLook::GetHeadCreatureAnimFrame (see src/00752510.cpp): 0.5 is centred, the angle is
// scaled by the def's turn range (degrees / 360), YZ picks the Up range for a positive angle, the result clamps to [0, 1].
#include <cstdio>
#include <cmath>
struct LookDef { float XY, Up, Down; };
static float Clamp(float v, float lo, float hi) { if (lo <= v && v <= hi) return v; if (v < lo) return lo; if (v > hi) return hi; return v; }
static float Frame(const LookDef& d, int anim, float xy, float yz)
{
    float frame;
    if (anim == 11) { float range = d.XY * 0.0027777778f; frame = xy * 0.5f / (range * 0.5f); }
    else frame = yz / (yz > 0.0f ? d.Up * 0.0027777778f : d.Down * 0.0027777778f) * 0.5f;
    frame = 0.5f - frame;
    return Clamp(frame, 0.0f, 1.0f);
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    LookDef d = { 180.0f, 90.0f, 45.0f };
    if (!Near(Frame(d, 11, 0.0f, 0.0f), 0.5f)) { std::printf("00752510_TEST FAIL\n"); return 1; }
    if (!Near(Frame(d, 11, 0.25f, 0.0f), 0.0f)) { std::printf("00752510_TEST FAIL\n"); return 1; }   // 0.5 - 0.25/0.5
    if (!Near(Frame(d, 11, -0.1f, 0.0f), 0.7f)) { std::printf("00752510_TEST FAIL\n"); return 1; }
    if (!Near(Frame(d, 12, 0.0f, 0.125f), 0.25f)) { std::printf("00752510_TEST FAIL\n"); return 1; }  // up range 0.25: 0.5 - 0.25
    if (!Near(Frame(d, 12, 0.0f, -0.0625f), 0.75f)) { std::printf("00752510_TEST FAIL\n"); return 1; } // down range 0.125: 0.5 + 0.25
    if (!Near(Frame(d, 12, 0.0f, 9.0f), 0.0f)) { std::printf("00752510_TEST FAIL\n"); return 1; }    // clamp low
    std::printf("00752510_TEST PASS\n");
    return 0;
}