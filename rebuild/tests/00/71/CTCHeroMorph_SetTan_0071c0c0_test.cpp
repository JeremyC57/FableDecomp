// behaviour test for 0x0071c0c0 CTCHeroMorph::SetTan(float): the value is clamped to [0, 1] (GFLimitReturn(v, 0, 1))
// into Tan and the morph is flagged Changed
#include <cstdio>
template <class T> static const T& Limit(const T& v, const T& lo, const T& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct Morph {
    bool Changed;
    float Tan;
    Morph() : Changed(false), Tan(0.5f) {}
    void SetTan(float v) { Tan = Limit(v, 0.0f, 1.0f); Changed = true; }
};
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    int fails = 0;
    Morph m;
    m.SetTan(0.25f);
    if (!Near(m.Tan, 0.25f) || !m.Changed) fails |= 1;
    m.Changed = false;
    m.SetTan(-3.0f);
    if (!Near(m.Tan, 0.0f) || !m.Changed) fails |= 2;
    m.SetTan(7.5f);
    if (!Near(m.Tan, 1.0f)) fails |= 4;
    m.SetTan(1.0f);
    if (!Near(m.Tan, 1.0f)) fails |= 8;
    m.SetTan(0.0f);
    if (!Near(m.Tan, 0.0f)) fails |= 16;
    if (fails) { std::printf("0071c0c0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071c0c0_TEST PASS\n");
    return 0;
}