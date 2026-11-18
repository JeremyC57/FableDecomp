// CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete() const -- how far the hero is into the current combo-multiplier level: the fractional part of (param / param-per-level + 1), floored at zero, in param units divided by param-per-level
// Leaf: ?GetExperiencePickupMultiplierCurrentLevelFractionComplete@CTCHeroExperience@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D63F0, 68 bytes, thiscall const, plain `ret`, float in st(0).
// Identity: Ego_d.pdb tc_hero_experience.obj, right after GetRunningNumHits (0x006D63A0); its locals diff, param_per_level,
//   multiplier (all float). Ego_r.pdb 0x00587B20 / FableWin.pdb 0x01EEE540 ?GetExperiencePickupMultiplierCurrentLevelFractionComplete@
//   CTCHeroExperience@@QBEMXZ right after ?GetRunningNumHits@CTCHeroExperience@@QBEJXZ. One caller: CDrawCombatMultiplier::Draw
//   (0x006482B0). The catalog label "Math_FracPart" is made up.
// Callees: __ftol2 0x00BFEA70. Constants 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: engine/CTCHeroExperience.h (ExperienceMultiplierParam +0x30, PDef +0x54), engine/CHeroExperienceDef.h
//   (ExperienceMultiplierParamPerUnitComboMultiplier +0x64).
// Shape: the same text as the landed sibling GetRunningNumHits 0x006D63A0 (VC7.1 shares the reciprocal of param_per_level for
//   both divisions: `fld [def+64h] ; fdivr 1.0f`). The final division is an in-place `diff /= param_per_level` followed by
//   `return diff;`: with `return diff / param_per_level;` VC7.1 tail-duplicates the division into the clamped path and folds it to
//   `fmul 0.0f` (64 bytes); retail merges the clamp (0x006D6424 fld 0.0f) and multiplies once (0x006D642A..0x006D6430).
// FITTED: the in-place `diff /= param_per_level; return diff;` was chosen by grading (a plain portable spelling, no device);
//   EVIDENCED: the three locals, their names / types (Ego_d) and the GetRunningNumHits statement order (landed twin).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_FRACTIONCOMPLETE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CHeroExperienceDef.h"

float CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete() const
{
    float param_per_level = PDef->ExperienceMultiplierParamPerUnitComboMultiplier;
    float multiplier = ExperienceMultiplierParam / param_per_level + 1.0f;
    float diff = multiplier - (long)multiplier;
    if (diff < 0.0f)
        diff = 0.0f;
    diff /= param_per_level;
    return diff;
}