// CTCHeroStats::Copy(const CTCBase& other) -- copy another hero-stats component: morality, age, suntan, fatness and money through their setters when the global apply flag is set (and the clothing stats marked stale), else directly; then the confiscated money, renown, health/stamina extras, appearance traits, experience and ability-use vectors, spent experience and chicken-throw record; then CTCBase::Copy
// Leaf: ?Copy@CTCHeroStats
//
// Retail 0x0057C9C4, 293 bytes (`ret 4`; CTCHeroStats vtable slot 12, catalog "vfunc_12"). Ego_r.pdb
// `?Copy@CTCHeroStats@@UAEXABVCTCBase@@@Z`. Size TU (`and dword ptr [m],0`, `push ecx ; fstp [esp]` float arguments):
// `#pragma optimize("s", on)` like the landed SetMoney 0x0057A774.
// Callees: CTCHeroStats::SetMorality(long) @ 0x0057C2E6, SetAge(float) @ 0x0057B05F, SetSunTan(float) @ 0x0057B0F7,
// SetFatness(float) @ 0x0057B169, SetMoney(long, bool) @ 0x0057A774, vector<long>::operator= @ 0x00454886 (twice),
// CTCBase::Copy(const CTCBase&) @ 0x006868D0. Global: the byte flag @ 0x013B8A88 (name descriptive).
// Layout: retail CTCHeroStats is 8 bytes below Ego_r.pdb after its first two containers and 4 more below each later
// 16-byte PDB vector (engine/CTCHeroStats.h carries the PDB offsets, so the model is local): Morality +0x28, Age +0x30,
// SunTan +0x34, Fatness +0x38, Money +0x3c, ConfiscatedMoney +0x44, Stamina +0x58, MaxStamina +0x5c,
// FramesUntilStaminaRefills +0x60, RenownLevel +0x70, RenownPointsInLevel +0x78, RenownTotal +0x7c, Agreeableness +0x80,
// InherentAttractiveness +0x84, InherentScariness +0x88, ExtraMaxHealth +0x8c, ExtraMaxStamina +0x90,
// FrameUpdatedClothingStats +0xb0, MaxChickenThrow +0x110, HeroStatsExperience +0x118, RegionTime +0x124,
// NumberOfTimesUsedAbility +0x130, TotalSpentExperience +0x13c.
// Shape: `other` cast to the derived reference once; the scalar copies in retail order (body statements).
#pragma optimize("s", on)

class CThing;
extern bool g_bApplyCopiedHeroStats;                        // 0x013B8A88

struct CLongVector {                                        // std::vector<long> (12 bytes)
    long* m_first; long* m_last; long* m_end;
    CLongVector& operator=(const CLongVector& other);      // 0x00454886
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual void Copy(const CTCBase& other);                // 0x006868D0
};

struct CTCHeroStats : CTCBase {
    unsigned char _pad0c[0x28 - 0x0c];                      // OutstandingCrimes map, ExperienceSpentOn vector, decapitations
    long  Morality;                                         // +0x28
    bool  MoralityChangingEnabled;                          // +0x2c
    float Age;                                              // +0x30
    float SunTan;                                           // +0x34
    float Fatness;                                          // +0x38
    long  Money;                                            // +0x3c
    long  MaxMoney;                                         // +0x40
    long  ConfiscatedMoney;                                 // +0x44
    long  TotalMoneyEarned;                                 // +0x48
    long  TotalMoneySpent;                                  // +0x4c
    long  RewardMoney;                                      // +0x50
    long  RentMoney;                                        // +0x54
    long  Stamina;                                          // +0x58
    long  MaxStamina;                                       // +0x5c
    long  FramesUntilStaminaRefills;                        // +0x60
    long  FramesUntilHealthRefills;                         // +0x64
    bool  ScriptOverridePreventStaminaRefill;               // +0x68
    long  FramesUntilFatnessReduces;                        // +0x6c
    long  RenownLevel;                                      // +0x70
    long  RenownMaxLevel;                                   // +0x74
    long  RenownPointsInLevel;                              // +0x78
    long  RenownTotal;                                      // +0x7c
    float Agreeableness;                                    // +0x80
    float InherentAttractiveness;                           // +0x84
    float InherentScariness;                                // +0x88
    float ExtraMaxHealth;                                   // +0x8c
    long  ExtraMaxStamina;                                  // +0x90
    unsigned char _pad94[0xb0 - 0x94];
    long  FrameUpdatedClothingStats;                        // +0xb0
    unsigned char _padb4[0x110 - 0xb4];
    float MaxChickenThrow;                                  // +0x110
    long  TrainingStatus;                                   // +0x114 EHeroTrainingStatus
    CLongVector HeroStatsExperience;                        // +0x118
    CLongVector RegionTime;                                 // +0x124
    CLongVector NumberOfTimesUsedAbility;                   // +0x130
    long  TotalSpentExperience;                             // +0x13c
    void SetMorality(long morality);                        // 0x0057C2E6
    void SetAge(float age);                                 // 0x0057B05F
    void SetSunTan(float sunTan);                           // 0x0057B0F7
    void SetFatness(float fatness);                         // 0x0057B169
    void SetMoney(long amount, bool countTransaction);      // 0x0057A774
    virtual void Copy(const CTCBase& other);
};

void CTCHeroStats::Copy(const CTCBase& other)
{
    const CTCHeroStats& stats = static_cast<const CTCHeroStats&>(other);
    if (g_bApplyCopiedHeroStats) {
        SetMorality(stats.Morality);
        SetAge(stats.Age);
        SetSunTan(stats.SunTan);
        SetFatness(stats.Fatness);
        SetMoney(stats.Money, true);
        FrameUpdatedClothingStats = 0;
    } else {
        Morality = stats.Morality;
        Age = stats.Age;
        SunTan = stats.SunTan;
        Fatness = stats.Fatness;
        Money = stats.Money;
    }
    ConfiscatedMoney = stats.ConfiscatedMoney;
    RenownLevel = stats.RenownLevel;
    RenownTotal = stats.RenownTotal;
    RenownPointsInLevel = stats.RenownPointsInLevel;
    ExtraMaxHealth = stats.ExtraMaxHealth;
    ExtraMaxStamina = stats.ExtraMaxStamina;
    MaxStamina = stats.MaxStamina;
    Stamina = stats.Stamina;
    FramesUntilStaminaRefills = stats.FramesUntilStaminaRefills;
    InherentAttractiveness = stats.InherentAttractiveness;
    InherentScariness = stats.InherentScariness;
    Agreeableness = stats.Agreeableness;
    HeroStatsExperience = stats.HeroStatsExperience;
    TotalSpentExperience = stats.TotalSpentExperience;
    MaxChickenThrow = stats.MaxChickenThrow;
    NumberOfTimesUsedAbility = stats.NumberOfTimesUsedAbility;
    CTCBase::Copy(other);
}
