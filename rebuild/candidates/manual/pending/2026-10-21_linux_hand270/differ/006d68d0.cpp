// CTCHeroExperience::IsBlockRecoilAnimToConserveAnimationHandedness() const -- true when the hero's weighted skill and strength stat fractions both exceed the combat def's maintain-handedness thresholds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D68D0, length 495 bytes.
// Leaf: IsBlockRecoilAnimToConserveAnimationHandedness
// Grade: DIFFER(495v495) -- one scheduling swap at the stat-5 clamp join: retail stores the clamped max (`mov [esp+0Ch],eax`) before `fmul [0.3f]`, ours after; everything else byte-identical (advisor v16/t16 shape)
// Names: catalogued "IsBlockRecoilAnimToConserveAnimationComboChain" (propagated). Ego_r.pdb order is ...Handedness 0x588120,
// ...ComboChain 0x588350, IsToAutoBlock 0x588580; CHeroCombatDef (retail = PDB - 0x20 after the 12-byte weighting vectors):
// AutoBlockSkill/Strength +0x104/+0x108, MaintainHandedness +0x10C/+0x110, MaintainComboChain +0x114/+0x118. This body
// compares the thresholds named in the source.
// Shape: six inline trainable-stat fractions (`long level`, clamped `long maxLevel`, then `float f = (float)level; return f /
// maxLevel;`), skill = (f3 + f4) * 0.3 + f5 * 0.4, strength = (f0 + f1) * 0.3 + f2 * 0.4, both `>` the thresholds; the combat
// def is a counted CDefPointer copy released at both exits.
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer()
    {
        if (Object)
            if (--Object->m_count == 0)
                Object->Destroy();
    }
    T* operator->() const { return Object; }
};

template<class T> struct CRetailVector {
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    T* begin() const { return _Myfirst; }
    T* end() const { return _Mylast; }
    unsigned long size() const { return _Mylast - _Myfirst; }
    T& operator[](unsigned long i) const { return _Myfirst[i]; }
};

struct CRetailList { void* _Myhead; };

struct CHeroCombatDef : CDefPointeeBase {
    unsigned char _pad08[0xfc];
    float AutoBlockSkillThreshold;                          // +0x104
    float AutoBlockStrengthThreshold;                       // +0x108
    float MeleeRecoilMaintainHandednessSkillThreshold;      // +0x10c
    float MeleeRecoilMaintainHandednessStrengthThreshold;   // +0x110
    float MeleeRecoilMaintainComboChainSkillThreshold;      // +0x114
    float MeleeRecoilMaintainComboChainStrengthThreshold;   // +0x118
};

struct CAbilityTrackLevelDef {
    unsigned char _base[0x3c];
    long ExperienceCost;                        // +0x3c
    long AbilityUnlocked;                       // +0x40
    long Description;                           // +0x44
};

struct CAbilityTrackDef {
    unsigned char _base[0x3c];
    long ExperienceStat;                        // +0x3c
    CRetailVector<CAbilityTrackLevelDef> Abilities;   // +0x40
    long MaxedOutDescription;                   // +0x4c
};

struct CHeroStatsDef {
    unsigned char _pad00[0xfc];
    CRetailVector<CAbilityTrackDef> StatLevels;             // +0xfc
};

struct CThing;
struct CHeroExperienceDef;
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };

class CTCHeroExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0c
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    CRetailVector<long> StatExperiencePoints;               // +0x18
    CRetailVector<long> TrainableStatLevels;                // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3c
    CRetailList Pickups;                                    // +0x40
    CRetailVector<long> ExperienceSpentOnStats;             // +0x44
    CRetailList AttackHistory;                              // +0x50
    CHeroExperienceDef* PDef;                               // +0x54
    CDefPointer<CHeroCombatDef> PHeroCombatDef;             // +0x58
    CHeroStatsDef* PHeroStatsDef;                           // +0x5c

    long GetTrainableStatLevel(EHeroTrainableStatType stat) const { return TrainableStatLevels[stat]; }
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const
    {
        const CRetailVector<CAbilityTrackLevelDef>& abilities = PHeroStatsDef->StatLevels[stat].Abilities;
        long maxLevel = (abilities.end() - abilities.begin()) - 1;
        if (maxLevel < 0)
            maxLevel = 0;
        return maxLevel;
    }
    __forceinline float GetFraction(long stat) const
    {
        long level = GetTrainableStatLevel((EHeroTrainableStatType)stat);
        long maxLevel = GetTrainableStatMaxLevel((EHeroTrainableStatType)stat);
        float f = (float)level;
        return f / maxLevel;
    }
    bool IsBlockRecoilAnimToConserveAnimationHandedness() const;
};

bool CTCHeroExperience::IsBlockRecoilAnimToConserveAnimationHandedness() const
{
    CDefPointer<CHeroCombatDef> combat = PHeroCombatDef;
    float f3 = GetFraction(3);
    float f4 = GetFraction(4);
    float f5 = GetFraction(5);
    float skill = (f3 + f4) * 0.3f + f5 * 0.4f;
    float f1 = GetFraction(1);
    float f0 = GetFraction(0);
    float f2 = GetFraction(2);
    float strength = (f0 + f1) * 0.3f + f2 * 0.4f;
    if (skill > combat->MeleeRecoilMaintainHandednessSkillThreshold && strength > combat->MeleeRecoilMaintainHandednessStrengthThreshold)
        return true;
    return false;
}
