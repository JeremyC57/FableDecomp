// Behaviour test for CTCHeroStats::Copy(const CTCBase&) (retail 0x0057C9C4): model copy + stub callees.
#include <cstdio>

class CThing;
bool g_bApplyCopiedHeroStats = false;
static int g_baseCopies = 0;
static int g_setterCalls = 0;
struct CLongVector {
    long v[2]; int assigned;
    CLongVector& operator=(const CLongVector& o) { v[0] = o.v[0]; v[1] = o.v[1]; ++assigned; return *this; }
};
class CTCBase { public:
    CThing* ParentThing;
    virtual void Copy(const CTCBase& other) { ParentThing = other.ParentThing; ++g_baseCopies; }
    virtual ~CTCBase() {}
};
class CTCHeroStats : public CTCBase { public:
    long  Morality; float Age; float SunTan; float Fatness; long Money; long ConfiscatedMoney;
    long  Stamina; long MaxStamina; long FramesUntilStaminaRefills;
    long  RenownLevel; long RenownPointsInLevel; long RenownTotal;
    float Agreeableness; float InherentAttractiveness; float InherentScariness; float ExtraMaxHealth; long ExtraMaxStamina;
    long  FrameUpdatedClothingStats; float MaxChickenThrow;
    CLongVector HeroStatsExperience; CLongVector RegionTime; CLongVector NumberOfTimesUsedAbility;
    long  TotalSpentExperience;
    bool  countedMoney;
    void SetMorality(long m) { Morality = m; ++g_setterCalls; }
    void SetAge(float a) { Age = a; ++g_setterCalls; }
    void SetSunTan(float s) { SunTan = s; ++g_setterCalls; }
    void SetFatness(float f) { Fatness = f; ++g_setterCalls; }
    void SetMoney(long amount, bool count) { Money = amount; countedMoney = count; ++g_setterCalls; }
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
static void Fill(CTCHeroStats& s, int k)
{
    s.ParentThing = (CThing*)0; s.Morality = 10 * k; s.Age = 1.5f * k; s.SunTan = 0.25f * k; s.Fatness = 0.5f * k; s.Money = 100 * k;
    s.ConfiscatedMoney = 7 * k; s.Stamina = 3 * k; s.MaxStamina = 4 * k; s.FramesUntilStaminaRefills = 5 * k; s.RenownLevel = 6 * k;
    s.RenownPointsInLevel = 8 * k; s.RenownTotal = 9 * k; s.Agreeableness = 0.1f * k; s.InherentAttractiveness = 0.2f * k;
    s.InherentScariness = 0.3f * k; s.ExtraMaxHealth = 11.0f * k; s.ExtraMaxStamina = 12 * k; s.FrameUpdatedClothingStats = 13 * k;
    s.MaxChickenThrow = 14.0f * k; s.HeroStatsExperience.v[0] = 15 * k; s.HeroStatsExperience.v[1] = 16 * k; s.HeroStatsExperience.assigned = 0;
    s.RegionTime.v[0] = 17 * k; s.RegionTime.v[1] = 0; s.RegionTime.assigned = 0;
    s.NumberOfTimesUsedAbility.v[0] = 18 * k; s.NumberOfTimesUsedAbility.v[1] = 19 * k; s.NumberOfTimesUsedAbility.assigned = 0;
    s.TotalSpentExperience = 20 * k; s.countedMoney = false;
}
int main()
{
    int fail = 0;
    CTCHeroStats a, b;
    Fill(a, 1); Fill(b, 2); b.ParentThing = (CThing*)&b;
    a.Copy(b);
    if (a.Morality != 20 || a.Age != 3.0f || a.Money != 200 || a.countedMoney || g_setterCalls != 0 || a.FrameUpdatedClothingStats != 13) fail = 1;
    if (a.ConfiscatedMoney != 14 || a.RenownTotal != 18 || a.ExtraMaxHealth != 22.0f || a.Stamina != 6 || a.Agreeableness != b.Agreeableness) fail = 2;
    if (a.HeroStatsExperience.v[1] != 32 || a.NumberOfTimesUsedAbility.v[0] != 36 || a.RegionTime.v[0] != 17 || a.TotalSpentExperience != 40 || a.MaxChickenThrow != 28.0f) fail = 3;
    if (g_baseCopies != 1 || a.ParentThing != (CThing*)&b) fail = 4;
    Fill(a, 1); g_bApplyCopiedHeroStats = true;
    a.Copy(b);
    if (g_setterCalls != 5 || !a.countedMoney || a.Money != 200 || a.Fatness != 1.0f || a.FrameUpdatedClothingStats != 0) fail = 5;
    if (fail) { std::printf("0057c9c4_TEST FAIL\n"); return 1; }
    std::printf("0057c9c4_TEST PASS\n");
    return 0;
}