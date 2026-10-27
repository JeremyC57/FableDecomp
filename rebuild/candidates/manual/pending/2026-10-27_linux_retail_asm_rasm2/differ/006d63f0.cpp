// CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete() const -- the fractional part of the pickup multiplier level (param per unit plus one), floored at zero and scaled by the per-unit reciprocal
// Leaf: ?GetExperiencePickupMultiplierCurrentLevelFractionComplete@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D63F0, 68 bytes (+8 int3), __thiscall, plain `ret` (one `push ecx` frame word for
//   the __ftol2 result). Callee: __ftol2 0x00BFEA70 (the `(long)level` cast: plain C++, no inline asm here -- the rasm1 pass
//   listed this row as not_asm). Constants: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC. Shape: the def's per-unit multiplier is
//   inverted once (`fld [def+64h] ; fdivr 1.0`) and the reciprocal is kept on the x87 stack for the level and for the result.
//   The catalog label "Math_FracPart" is descriptive; the Ego_r.pdb name is
//   CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete.
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
    float perUnit = 1.0f / PDef->ExperienceMultiplierParamPerUnitComboMultiplier;    // one reciprocal, shared by both scalings
    float level = ExperienceMultiplierParam * perUnit + 1.0f;
    float fraction = level - (long)level;
    return (fraction < 0.0f ? 0.0f : fraction) * perUnit;
}
