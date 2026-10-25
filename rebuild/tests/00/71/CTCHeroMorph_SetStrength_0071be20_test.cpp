// behaviour test for 0x0071be20 CTCHeroMorph::SetStrength(float): the value is clamped to [0, 1] (GFLimitReturn(v, 0, 1))
// into Strength and the morph is flagged Changed
#include <cstdio>
template <class T> static const T& Limit(const T& v, const T& lo, const T& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct Morph {
    bool Changed;
    float Strength;
    Morph() : Changed(false), Strength(0.5f) {}
    void SetStrength(float v) { Strength = Limit(v, 0.0f, 1.0f); Changed = true; }
};
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    int fails = 0;
    Morph m;
    m.SetStrength(0.25f);
    if (!Near(m.Strength, 0.25f) || !m.Changed) fails |= 1;
    m.Changed = false;
    m.SetStrength(-3.0f);
    if (!Near(m.Strength, 0.0f) || !m.Changed) fails |= 2;
    m.SetStrength(7.5f);
    if (!Near(m.Strength, 1.0f)) fails |= 4;
    m.SetStrength(1.0f);
    if (!Near(m.Strength, 1.0f)) fails |= 8;
    m.SetStrength(0.0f);
    if (!Near(m.Strength, 0.0f)) fails |= 16;
    if (fails) { std::printf("0071be20_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071be20_TEST PASS\n");
    return 0;
}