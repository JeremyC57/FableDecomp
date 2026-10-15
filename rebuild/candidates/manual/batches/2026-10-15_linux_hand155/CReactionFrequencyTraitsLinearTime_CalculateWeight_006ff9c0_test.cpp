// Behaviour test for 0x006FF9C0: LinearTime::CalculateWeight = clamp((frames since happened - MinWait) / WaitRange, 0, 1).
#include <cstdio>
static long g_frame;
static long GetWorldFrame() { return g_frame; }
static const float& Clamp(const float& v, const float& lo, const float& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct M { float MinWait, WaitRange; unsigned int FrameHappened;
  float CalculateWeight() const { float e = (float)(unsigned int)(GetWorldFrame() - FrameHappened); return Clamp((e - MinWait) / WaitRange, 0.0f, 1.0f); } };
int main()
{
    M m = {10.0f, 20.0f, 100};
    g_frame = 100; float a = m.CalculateWeight();   // -0.5 -> 0
    g_frame = 120; float b = m.CalculateWeight();   // 0.5
    g_frame = 200; float c = m.CalculateWeight();   // 4.5 -> 1
    g_frame = 50;  float d = m.CalculateWeight();   // unsigned wrap -> huge -> 1
    bool ok = a == 0.0f && b == 0.5f && c == 1.0f && d == 1.0f;
    if (!ok) { std::printf("006ff9c0_TEST FAIL\n"); return 1; }
    std::printf("006ff9c0_TEST PASS\n");
    return 0;
}
