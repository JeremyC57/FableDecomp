// Behaviour test for CTCCreatureOpinionOfHero::TransformForcedOpinion 0x00681790 (model copy of the source logic).
#include <cstdio>
#include <cmath>
struct Traits { float BaseOffset, HeroStatScaling, VillageOpinionScaling, TimeScalingRunIn, TimeScalingRunOut,
                PeakScalingIfPos, PeakScalingIfNeg, PersistScalingIfPos, PersistScalingIfNeg; };
struct Def { Traits* PersonalityTraits; };
struct DefPointer {
    Def* Object;
    bool IsNull() const { return Object == 0; }
    Def* operator->() const { return Object; }
};
struct Opinion {
    DefPointer PPersonalityDef;
    float TransformForcedOpinion(int opinion, float value) const
    {
        if (!PPersonalityDef.IsNull())
            return value * PPersonalityDef->PersonalityTraits[opinion].HeroStatScaling
                 + PPersonalityDef->PersonalityTraits[opinion].BaseOffset;
        return value;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Traits t[3] = {};
    t[0].BaseOffset = 1.0f;  t[0].HeroStatScaling = 2.0f;
    t[2].BaseOffset = -0.5f; t[2].HeroStatScaling = 0.25f; t[2].VillageOpinionScaling = 99.0f;
    Def d; d.PersonalityTraits = t;
    Opinion o; o.PPersonalityDef.Object = 0;
    if (!near(o.TransformForcedOpinion(2, 3.5f), 3.5f)) { std::printf("00681790_TEST FAIL\n"); return 1; }
    o.PPersonalityDef.Object = &d;
    if (!near(o.TransformForcedOpinion(0, 3.0f), 7.0f)) { std::printf("00681790_TEST FAIL\n"); return 1; }
    if (!near(o.TransformForcedOpinion(2, 4.0f), 0.5f)) { std::printf("00681790_TEST FAIL\n"); return 1; }
    if (!near(o.TransformForcedOpinion(1, 4.0f), 0.0f)) { std::printf("00681790_TEST FAIL\n"); return 1; }
    std::printf("00681790_TEST PASS\n");
    return 0;
}