// behaviour test for 0x0071bf40 CTCHeroMorph::SetSkill(float): the value is clamped to [0, 1] (GFLimitReturn(v, 0, 1))
// into Skill and the morph is flagged Changed
#include <cstdio>
template <class T> static const T& Limit(const T& v, const T& lo, const T& hi) { if (v < lo) return lo; if (v > hi) return hi; return v; }
struct Morph {
    bool Changed;
    float Skill;
    Morph() : Changed(false), Skill(0.5f) {}
    void SetSkill(float v) { Skill = Limit(v, 0.0f, 1.0f); Changed = true; }
};
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    int fails = 0;
    Morph m;
    m.SetSkill(0.25f);
    if (!Near(m.Skill, 0.25f) || !m.Changed) fails |= 1;
    m.Changed = false;
    m.SetSkill(-3.0f);
    if (!Near(m.Skill, 0.0f) || !m.Changed) fails |= 2;
    m.SetSkill(7.5f);
    if (!Near(m.Skill, 1.0f)) fails |= 4;
    m.SetSkill(1.0f);
    if (!Near(m.Skill, 1.0f)) fails |= 8;
    m.SetSkill(0.0f);
    if (!Near(m.Skill, 0.0f)) fails |= 16;
    if (fails) { std::printf("0071bf40_TEST FAIL %d\n", fails); return 1; }
    std::printf("0071bf40_TEST PASS\n");
    return 0;
}