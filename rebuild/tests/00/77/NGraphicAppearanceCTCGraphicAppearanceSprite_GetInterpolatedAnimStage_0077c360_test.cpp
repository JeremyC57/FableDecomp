// Behaviour test for 0x0077C360 CLoopedRange::GetFraction(t)
#include <cstdio>

const float g_epsilon_0129ba3c = 0.0001f;
struct CLoopedRange {
    unsigned char _pad_0x00[0x1C];
    float End;
    float Start;
    float GetFraction(float t) const
    {
        float r;
        if (End > Start)
            r = (End - Start) * t + Start;
        else {
            float s = 1.0f - Start;
            r = s + (End - s) * t;
        }
        if (r < 0.0f)
            return 0.0f;
        const float maxFraction = 1.0f - g_epsilon_0129ba3c;
        if (r > maxFraction)
            return maxFraction;
        return r;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.00001f && d > -0.00001f; }

int main()
{
    CLoopedRange r;
    r.Start = 0.25f; r.End = 0.75f;
    if (!Near(r.GetFraction(0.0f), 0.25f) || !Near(r.GetFraction(0.5f), 0.5f)) { std::printf("0077c360_TEST FAIL\n"); return 1; }
    if (!Near(r.GetFraction(1.0f), 0.75f)) { std::printf("0077c360_TEST FAIL\n"); return 1; }
    if (!Near(r.GetFraction(4.0f), 1.0f - 0.0001f)) { std::printf("0077c360_TEST FAIL\n"); return 1; }   // clamped high
    if (r.GetFraction(-4.0f) != 0.0f) { std::printf("0077c360_TEST FAIL\n"); return 1; }                // clamped low
    r.Start = 0.75f; r.End = 0.5f;                          // end below start: s = 0.25, r = 0.25 + 0.25 t
    if (!Near(r.GetFraction(0.0f), 0.25f) || !Near(r.GetFraction(1.0f), 0.5f)) { std::printf("0077c360_TEST FAIL\n"); return 1; }
    std::printf("0077c360_TEST PASS\n");
    return 0;
}