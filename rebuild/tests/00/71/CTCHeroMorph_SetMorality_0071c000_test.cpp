// behaviour test for 0x0071c000 CTCHeroMorph::SetMorality(float): the value is clamped to [0, 1] (GFLimitReturn(v, 0, 1))
// into Morality and the morph is flagged Changed
#include <cstdio>
template <class T> static const T& Limit(const T& v, const T& lo, const T& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct Morph {
    bool Changed;
    float Morality;
    Morph() : Changed(false), Morality(0.5f) {}
    void SetMorality(float v) { Morality = Limit(v, 0.0f, 1.0f); Changed = true; }
};
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    int fails = 0;
    Morph m;
    m.SetMorality(0.25f);
    if (!Near(m.Morality, 0.25f) || !m.Changed) fails |= 1;
    m.Changed = false;
    m.SetMorality(-3.0f);
    if (!Near(m.Morality, 0.0f) || !m.Changed) fails |= 2;
    m.SetMorality(7.5f);
    if (!Near(m.Morality, 1.0f)) fails |= 4;
    m.SetMorality(1.0f);
    if (!Near(m.Morality, 1.0f)) fails |= 8;
    m.SetMorality(0.0f);
    if (!Near(m.Morality, 0.0f)) fails |= 16;
    if (fails) { std::printf("0071c000_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071c000_TEST PASS\n");
    return 0;
}