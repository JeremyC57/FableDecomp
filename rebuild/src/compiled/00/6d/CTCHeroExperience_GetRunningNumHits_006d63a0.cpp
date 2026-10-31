// CTCHeroExperience::GetRunningNumHits() const -- how many hits the hero has landed towards the next combo multiplier step: the fractional part of the current multiplier scaled back to hits
// Leaf: ?GetRunningNumHits@CTCHeroExperience@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D63A0, 77 bytes, thiscall, `ret`.
// Identity (proven): caller 0x00899D3E is CGameScriptInterface::GetPlayerCreatureCombatMultiplierRunningNumHits, and the
//   Ego_r.pdb donor order of CTCHeroExperience (ApplyPenaltyForDeath, IsFlourishPending, IsAbleToBreakDownDoors,
//   SetAsFlourished, GetRunningNumHits, ...) matches retail (landed ApplyPenaltyForDeath 0x006D6340,
//   IsAbleToBreakDownDoors 0x006D6370, then this row); ?GetRunningNumHits@CTCHeroExperience@@QBEJXZ is the long() const.
//   The other callers 0x006D73DB / 0x006D80B2 are CTCHeroExperience code. Label correction: the catalog label
//   "CEngine::GetEditorAnimation" is propagated (that PDB function takes a CWideString).
// Callees: _ftol 0x00BFEA70 (twice). Float pool: 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 4.0f 0x0123088C.
// Layout: engine/CTCHeroExperience.h (ExperienceMultiplierParam +0x30, PDef_Object +0x54, the landed
//   GetActiveComboMultiplierData 0x006D72D0 pattern); CHeroExperienceDef::ExperienceMultiplierParamPerUnitComboMultiplier
//   +0x64 (retail offset, Ego_r name).
// Shape: both the multiplier (param / perUnit + 1) and the hit count (fraction / perUnit * 4) DIVIDE by the per-unit
//   param held in a named float local. VC7.1 turns the two divisions by one register value into a single reciprocal
//   (`fld [def+64h] ; fdivr 1.0`, the divisor already on the x87 stack) kept for both products. A reciprocal written
//   by hand (`1.0f / def->x`, inline helper, named or reference local) or two divisions by the member itself give
//   `fld 1.0 ; fdiv [def+64h]`.
#define CTCHeroExperience CTCHeroExperienceLayout
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience

class CHeroExperienceDef {                              // retail offsets, Ego_r names
public:
    unsigned char _pad00[0x64];                         // def base, vectors, SoundIntensityThresholds, ComboMultiplierMap
    float ExperienceMultiplierParamPerUnitComboMultiplier;      // +0x64
};

class CTCHeroExperience : public CTCHeroExperienceLayout {
public:
    long GetRunningNumHits() const;
};

long CTCHeroExperience::GetRunningNumHits() const
{
    float perUnit = ((const CHeroExperienceDef*)PDef_Object)->ExperienceMultiplierParamPerUnitComboMultiplier;
    float multiplier = ExperienceMultiplierParam / perUnit + 1.0f;
    float fraction = multiplier - (long)multiplier;
    if (fraction < 0.0f)
        fraction = 0.0f;
    return (long)(fraction / perUnit * 4.0f);
}