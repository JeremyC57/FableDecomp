// Behaviour test for CThingCreatureBase::GetCurrentMeshRadius (0x00664BA0), a model of the row: 1 when the thing has no hero-experience
// component or its creature def has no CCreatureDef sub-def; otherwise LowValue + (HighValue - LowValue) * param, where param is the
// def's RadiusWeight.GetParameter(experience) (a stub here). Dyadic values keep the arithmetic exact.
#include <stdio.h>
struct CTCHeroExperience { float Level; };
struct CHeroAbilityStatWeight { float Scale; float GetParameter(const CTCHeroExperience& e) const { return e.Level * Scale; } };
struct CFloatRange { float HighValue, LowValue; };
struct CCreatureDef { CHeroAbilityStatWeight RadiusWeight; CFloatRange RadiusSpread; };
struct Thing { const CTCHeroExperience* Experience; const CCreatureDef* SubDef; };
static float GetCurrentMeshRadius(const Thing& t)
{
    const CTCHeroExperience* ptc_hero_experience = t.Experience;
    if (!ptc_hero_experience)
        return 1.0f;
    const CCreatureDef* pcreature_def = t.SubDef;
    if (!pcreature_def)
        return 1.0f;
    float param = pcreature_def->RadiusWeight.GetParameter(*ptc_hero_experience);
    float rad = pcreature_def->RadiusSpread.LowValue + (pcreature_def->RadiusSpread.HighValue - pcreature_def->RadiusSpread.LowValue) * param;
    return rad;
}
int main()
{
    int bad = 0;
    CTCHeroExperience e = { 2.0f };
    CCreatureDef d = { { 0.25f }, { 3.0f, 1.0f } };
    Thing none = { 0, &d }, nodef = { &e, 0 }, full = { &e, &d };
    if (GetCurrentMeshRadius(none) != 1.0f) ++bad;
    if (GetCurrentMeshRadius(nodef) != 1.0f) ++bad;
    if (GetCurrentMeshRadius(full) != 1.0f + 2.0f * 0.5f) ++bad;              // param 0.5: halfway between 1 and 3
    printf(bad ? "00664ba0_TEST FAIL\n" : "00664ba0_TEST PASS\n");
    return bad ? 1 : 0;
}
