// Behaviour test for 0x00700200: SecondaryAxisCentredMRShifted::Match = clamp(max(1 - |o[axis]-C|/R, 0) + M + R shifts, 0, 1).
#include <cstdio>
#include <math.h>
static const float& Clamp(const float& v, const float& lo, const float& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct M { long Axis; float C, Rad, MZ, MW, RZ, RW;
  float Match(const float* o) const { float t = 1.0f - (float)fabs(o[Axis] - C) / Rad; if (t < 0.0f) t = 0.0f;
    float r = t + (o[0] - MZ) * MW + (o[1] - RZ) * RW; return Clamp(r, 0.0f, 1.0f); } };
int main()
{
    float o[3] = {1.0f, 0.5f, 0.75f};
    M a = {2, 0.25f, 2.0f, 1.0f, 1.0f, 0.5f, 1.0f};    // 1 - 0.25 = 0.75
    M b = {2, 5.0f, 1.0f, 0.0f, 0.25f, 0.0f, 0.0f};    // floor 0 + 0.25 = 0.25
    M c = {2, 0.75f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};    // 1 + 1 -> 1
    bool ok = a.Match(o) == 0.75f && b.Match(o) == 0.25f && c.Match(o) == 1.0f;
    if (!ok) { std::printf("00700200_TEST FAIL\n"); return 1; }
    std::printf("00700200_TEST PASS\n");
    return 0;
}