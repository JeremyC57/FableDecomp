// Behaviour test for 0x00700180: SecondaryAxisMRShifted::Match = clamp((o[axis]-L)*I + (o[0]-MZ)*MW + (o[1]-RZ)*RW, 0, 1).
#include <cstdio>
static const float& Clamp(const float& v, const float& lo, const float& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct M { long Axis; float L, I, MZ, MW, RZ, RW;
  float Match(const float* o) const { return Clamp((o[Axis] - L) * I + (o[0] - MZ) * MW + (o[1] - RZ) * RW, 0.0f, 1.0f); } };
int main()
{
    float o[3] = {1.0f, 0.5f, 0.75f};
    M a = {2, 0.25f, 0.5f, 1.0f, 1.0f, 0.5f, 1.0f};   // 0.25 + 0 + 0 = 0.25
    M b = {2, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f};    // 1.5 -> 1
    M c = {2, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};    // -0.25 -> 0
    bool ok = a.Match(o) == 0.25f && b.Match(o) == 1.0f && c.Match(o) == 0.0f;
    if (!ok) { std::printf("00700180_TEST FAIL\n"); return 1; }
    std::printf("00700180_TEST PASS\n");
    return 0;
}
