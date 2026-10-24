// CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete() const -- the fractional part of the pickup multiplier level (param per unit plus one), floored at zero and scaled by the per-unit reciprocal
// Leaf: ?GetExperiencePickupMultiplierCurrentLevelFractionComplete@
// Grade: DIFFER(64v68) -- x87 scheduling only: retail forms the shared reciprocal as `fld [def+64h] ; fdivr 1.0f` and
//        keeps the clamp as a merge (the clamped path loads 0.0f, then one `fxch ; fmul st(1) ; fxch ; fstp st(0)` tail);
//        ours emits `fld 1.0f ; fdiv [def+64h]` and folds the clamped path to `fmul 0.0f` (two tails). Tried: an explicit
//        reciprocal local, a named perUnit local with a `?:` clamp, the two-division spelling (VC7.1 CSEs the reciprocal
//        either way; identical code), STLport `max` (reference select, 76 bytes). next: a by-value float helper
//        (e.g. an inline `GetExperienceMultiplierParamForMultiplier`-style accessor) feeding the division.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D63F0, length 68 bytes, thiscall plain `ret`, float in st(0). Ego_r.pdb
// `?GetExperiencePickupMultiplierCurrentLevelFractionComplete@CTCHeroExperience@@QBEMXZ`; single caller 0x006482B0 in
// CDrawCombatMultiplier::Draw (0x0064822A), the combo-multiplier HUD. The manifest label "Math_FracPart" is descriptive.
// Callees: __ftol2 @ 0x00BFEA70 (truncation; not a bare fistp). Constants: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Layout: CTCHeroExperience retail (ExperienceMultiplierParam +0x30, PDef +0x54); CHeroExperienceDef retail
// ExperienceMultiplierParamPerUnitComboMultiplier +0x64 (as in the landed 0x006D7F20).
// Shape: `level = param / perUnit + 1; fraction = level - (long)level` floored at 0, returned `/ perUnit`: VC7.1 CSEs
// the reciprocal `1 / perUnit`, which stays on the x87 stack for both uses; `level - (long)level` is the `fisub` of the
// __ftol2 result spilled to the frame word.
class CHeroExperienceDef {
public:
    unsigned char _base[0x64];                              // CDefClassBase, level/title/fallback vectors, maps
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
};

class CTCHeroExperience {
public:
    virtual void v00();
    unsigned char _pad04[0x2C];                             // ParentThing .. TrainableStatLevels
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    unsigned char _pad38[0x1C];                             // ExperienceAttenuationTimer .. AttackHistory
    CHeroExperienceDef* PDef;                               // +0x54

    float GetExperiencePickupMultiplierCurrentLevelFractionComplete() const;
};

float CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete() const
{
    float level = ExperienceMultiplierParam / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 1.0f;
    float fraction = level - (long)level;
    if (fraction < 0.0f)
        fraction = 0.0f;
    return fraction / PDef->ExperienceMultiplierParamPerUnitComboMultiplier;
}
