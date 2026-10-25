// behaviour test for 0x0071bfa0 CTCHeroMorph::SetAge(float): the value is clamped to [0, 1] (GFLimitReturn(v, 0, 1))
// into Age and the morph is flagged Changed
#include <cstdio>
template <class T> static const T& Limit(const T& v, const T& lo, const T& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct Morph {
    bool Changed;
    float Age;
    Morph() : Changed(false), Age(0.5f) {}
    void SetAge(float v) { Age = Limit(v, 0.0f, 1.0f); Changed = true; }
};
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    int fails = 0;
    Morph m;
    m.SetAge(0.25f);
    if (!Near(m.Age, 0.25f) || !m.Changed) fails |= 1;
    m.Changed = false;
    m.SetAge(-3.0f);
    if (!Near(m.Age, 0.0f) || !m.Changed) fails |= 2;
    m.SetAge(7.5f);
    if (!Near(m.Age, 1.0f)) fails |= 4;
    m.SetAge(1.0f);
    if (!Near(m.Age, 1.0f)) fails |= 8;
    m.SetAge(0.0f);
    if (!Near(m.Age, 0.0f)) fails |= 16;
    if (fails) { std::printf("0071bfa0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071bfa0_TEST PASS\n");
    return 0;
}