// CTCHeroExperience::SetAsHit() -- the hero was hit: the combo multiplier drops (flourish invalidated, the "lost" sound when it was above one), the attenuation timer rearms and the param falls back to the current fallback multiplier's start
// Leaf: ?SetAsHit@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D80E0, length 134 bytes (+ int3 padding), thiscall plain `ret`. Ego_r.pdb
// `?SetAsHit@CTCHeroExperience@@QAEXXZ`; single caller 0x0083A44B in CThingGameObject::OnHit (0x00839940), which fetches
// the hero's experience component (interface 0x68) first. The manifest label "CTCHeroExperience_OnComboHit" is wrong.
// Callees: OnComboMultiplierDecrease() @ 0x006D7E70 (landed), __ftol2 @ 0x00BFEA70 (x3), the file-local
// PlayCombatMultiplierSound(CThing&, const map<long, CDefString>&, long) @ 0x006D7640 (this chunk, `__fastcall`),
// GetActiveComboMultiplierData() const @ 0x006D72D0, GetExperienceFallbackMultiplierForMultiplier(long) const
// @ 0x006D73D0 (this chunk). Constants: 0.5 (double) @ 0x01237F00; the integer frame rate @ 0x01375550.
// Layout: CTCHeroExperience retail (ParentThing +0x04, ExperienceMultiplierParam +0x30, ExperienceAttenuationTimer +0x38,
// PDef +0x54); CHeroExperienceDef retail: ExperienceMultiplierParamPerUnitComboMultiplier +0x64, ComboMultiplierLost
// map<long, CDefString> +0x84 (as in the landed 0x006D7F20); CComboMultiplierData NumSecondsForAttenuation +0x0C.
// Shape: the inline GetExperiencePickUpMultiplier (`(long)(param / perUnit + 0.5)` clamped to 1) of the landed 0x006D7F20
// into a named local -- the clamp then folds into the `> 1` test (calling the accessor twice keeps a second clamp), then the timer as in AddSuccessfulStrikeOfType, then the param set through the
// inline GetExperienceMultiplierParamForMultiplier(GetExperienceFallbackMultiplier()) = `(fallback - 1) * perUnit`.
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

class CThing;

class CComboMultiplierSounds { public: unsigned char _map[0xC]; };   // map<long, CDefString>

void __fastcall PlayCombatMultiplierSound(CThing& thing, const CComboMultiplierSounds& sounds, long multiplier);   // 0x006D7640

class CComboMultiplierData {
public:
    unsigned char ComboMultiplier[0xC];                     // +0x00 map<EDamageAttribute, float>
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0C
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};

class CHeroExperienceDef {
public:
    unsigned char _base[0x64];                              // CDefClassBase, level/title/fallback vectors, maps
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
    float ExperienceMultiplierRegionChangeMultiplier;       // +0x68
    CComboMultiplierSounds ComboMultiplierIncrease;         // +0x6C
    CComboMultiplierSounds ComboMultiplierLock;             // +0x78
    CComboMultiplierSounds ComboMultiplierLost;             // +0x84
};

extern long g_framesPerSecond;                              // 0x01375550

class CTCHeroExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0C
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    long* StatExperiencePoints[3];                          // +0x18
    long* TrainableStatLevels[3];                           // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3C
    void* Pickups;                                          // +0x40
    long* ExperienceSpentOnStats[3];                        // +0x44
    void* AttackHistory;                                    // +0x50
    CHeroExperienceDef* PDef;                               // +0x54

    long GetExperiencePickupMultiplierForParam(float param) const
    {
        long multiplier = (long)(param / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5);
        if (multiplier < 1)
            multiplier = 1;
        return multiplier;
    }
    long GetExperiencePickUpMultiplier() const { return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam); }
    float GetExperienceMultiplierParamForMultiplier(long multiplier) const
    {
        return (multiplier - 1) * PDef->ExperienceMultiplierParamPerUnitComboMultiplier;
    }
    long GetExperienceFallbackMultiplierForMultiplier(long multiplier) const;   // 0x006D73D0
    long GetExperienceFallbackMultiplier() const { return GetExperienceFallbackMultiplierForMultiplier(GetExperiencePickUpMultiplier()); }
    const CComboMultiplierData& GetActiveComboMultiplierData() const;   // 0x006D72D0
    void OnComboMultiplierDecrease();                       // 0x006D7E70
    void SetAsHit();
};

void CTCHeroExperience::SetAsHit()
{
    OnComboMultiplierDecrease();
    long multiplier = GetExperiencePickUpMultiplier();
    if (multiplier > 1)
        PlayCombatMultiplierSound(*ParentThing, PDef->ComboMultiplierLost, multiplier);
    ExperienceAttenuationTimer = (long)(g_framesPerSecond * GetActiveComboMultiplierData().ExperienceMultiplierNumSecondsForAttenuation);
    ExperienceMultiplierParam = GetExperienceMultiplierParamForMultiplier(GetExperienceFallbackMultiplier());
}