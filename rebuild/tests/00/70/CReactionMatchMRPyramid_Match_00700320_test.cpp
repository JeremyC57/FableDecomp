// Behaviour test for 0x00700320: MRPyramid::Match = clamp(base pyramid match + o[2]*S + o[3]*A + o[4]*T, 0, 1).
#include <cstdio>
static const float& Clamp(const float& v, const float& lo, const float& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct M { float base, S, A, T;
  float Match(const float* o) const { float r = base; r += o[2] * S; r += o[3] * A; r += o[4] * T; return Clamp(r, 0.0f, 1.0f); } };
int main()
{
    float o[5] = {0, 0, 1.0f, 0.5f, 0.25f};
    M a = {0.25f, 0.25f, 0.5f, 0.0f};   // 0.25 + 0.25 + 0.25 = 0.75
    M b = {0.5f, 1.0f, 0.0f, 0.0f};     // 1.5 -> 1
    M c = {0.0f, -1.0f, 0.0f, 0.0f};    // -1 -> 0
    bool ok = a.Match(o) == 0.75f && b.Match(o) == 1.0f && c.Match(o) == 0.0f;
    if (!ok) { std::printf("00700320_TEST FAIL\n"); return 1; }
    std::printf("00700320_TEST PASS\n");
    return 0;
}