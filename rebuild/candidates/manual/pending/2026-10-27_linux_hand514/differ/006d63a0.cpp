// CTCHeroExperience::GetRunningNumHits() const -- how many hits the hero has landed towards the next combo multiplier step: the fractional part of the current multiplier scaled back to hits
// Grade: DIFFER(77v77) -- 1.0f/def->x emits fld 1.0 ; fdiv [def+64h], retail fld [def+64h] ; fdivr 1.0 (reverse-operand form); rest matches
// Leaf: ?GetRunningNumHits@CTCHeroExperience@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D63A0, 77 bytes, thiscall, `ret`.
// Identity (proven): caller 0x00899D3E is CGameScriptInterface::GetPlayerCreatureCombatMultiplierRunningNumHits, and the
//   Ego_r.pdb donor order of CTCHeroExperience (ApplyPenaltyForDeath, IsFlourishPending, IsAbleToBreakDownDoors,
//   SetAsFlourished, GetRunningNumHits, ...) matches retail (landed ApplyPenaltyForDeath 0x006D6340,
//   IsAbleToBreakDownDoors 0x006D6370, then this row); ?GetRunningNumHits@CTCHeroExperience@@QBEJXZ is the long() const.
//   The other callers 0x006D73DB / 0x006D80B2 are CTCHeroExperience code. The catalog label
//   "CEngine::GetEditorAnimation" is propagated (that PDB function takes a CWideString).
// Callees: _ftol 0x00BFEA70 (twice). Float pool: 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 4.0f 0x0123088C.
// Layout: retail ExperienceMultiplierParam +0x30, PDef +0x54; CHeroExperienceDef::ExperienceMultiplierParamPerUnitComboMultiplier
//   +0x64 (the landed CTCHeroExperience models).
// Shape: the reciprocal of the per-unit param is an inline helper used twice; VC7.1 computes it once (`fdivr 1.0`) and
//   keeps the common subexpression on the FPU stack for both uses (a named local instead gives `fld 1.0 ; fdiv`).
class CHeroExperienceDef { public:
    unsigned char _pad00[0x64];
    float         ExperienceMultiplierParamPerUnitComboMultiplier;   // +0x64
};
class CTCHeroExperience {
public:
    unsigned char       _pad00[0x30];
    float               ExperienceMultiplierParam;                  // +0x30
    unsigned char       _pad34[0x54 - 0x34];
    CHeroExperienceDef* PDef;                                       // +0x54
    float GetComboMultiplierPerParam() const { return 1.0f / PDef->ExperienceMultiplierParamPerUnitComboMultiplier; }
    long GetRunningNumHits() const;
};

long CTCHeroExperience::GetRunningNumHits() const
{
    float multiplier = ExperienceMultiplierParam * GetComboMultiplierPerParam() + 1.0f;
    float fraction = multiplier - (long)multiplier;
    if (fraction < 0.0f)
        fraction = 0.0f;
    return (long)(GetComboMultiplierPerParam() * fraction * 4.0f);
}
