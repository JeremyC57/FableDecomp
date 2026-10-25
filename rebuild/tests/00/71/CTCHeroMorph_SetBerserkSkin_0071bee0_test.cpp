// behaviour test for 0x0071bee0 CTCHeroMorph::SetBerserkSkin(float): the value is clamped to [0, 1] (GFLimitReturn(v, 0, 1))
// into Berserk and the morph is flagged Changed
#include <cstdio>
template <class T> static const T& Limit(const T& v, const T& lo, const T& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct Morph {
    bool Changed;
    float Berserk;
    Morph() : Changed(false), Berserk(0.5f) {}
    void SetBerserkSkin(float v) { Berserk = Limit(v, 0.0f, 1.0f); Changed = true; }
};
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    int fails = 0;
    Morph m;
    m.SetBerserkSkin(0.25f);
    if (!Near(m.Berserk, 0.25f) || !m.Changed) fails |= 1;
    m.Changed = false;
    m.SetBerserkSkin(-3.0f);
    if (!Near(m.Berserk, 0.0f) || !m.Changed) fails |= 2;
    m.SetBerserkSkin(7.5f);
    if (!Near(m.Berserk, 1.0f)) fails |= 4;
    m.SetBerserkSkin(1.0f);
    if (!Near(m.Berserk, 1.0f)) fails |= 8;
    m.SetBerserkSkin(0.0f);
    if (!Near(m.Berserk, 0.0f)) fails |= 16;
    if (fails) { std::printf("0071bee0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071bee0_TEST PASS\n");
    return 0;
}