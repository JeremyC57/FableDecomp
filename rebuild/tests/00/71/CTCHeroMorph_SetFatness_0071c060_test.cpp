// behaviour test for 0x0071c060 CTCHeroMorph::SetFatness(float): the value is clamped to [0, 1] (GFLimitReturn(v, 0, 1))
// into Fatness and the morph is flagged Changed
#include <cstdio>
template <class T> static const T& Limit(const T& v, const T& lo, const T& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct Morph {
    bool Changed;
    float Fatness;
    Morph() : Changed(false), Fatness(0.5f) {}
    void SetFatness(float v) { Fatness = Limit(v, 0.0f, 1.0f); Changed = true; }
};
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    int fails = 0;
    Morph m;
    m.SetFatness(0.25f);
    if (!Near(m.Fatness, 0.25f) || !m.Changed) fails |= 1;
    m.Changed = false;
    m.SetFatness(-3.0f);
    if (!Near(m.Fatness, 0.0f) || !m.Changed) fails |= 2;
    m.SetFatness(7.5f);
    if (!Near(m.Fatness, 1.0f)) fails |= 4;
    m.SetFatness(1.0f);
    if (!Near(m.Fatness, 1.0f)) fails |= 8;
    m.SetFatness(0.0f);
    if (!Near(m.Fatness, 0.0f)) fails |= 16;
    if (fails) { std::printf("0071c060_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071c060_TEST PASS\n");
    return 0;
}