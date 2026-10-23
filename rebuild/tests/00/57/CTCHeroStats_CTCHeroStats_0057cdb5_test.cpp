// Behaviour test for CTCHeroStats::CTCHeroStats @ 0x0057CDB5 (default hero stats, sized tables, def-derived values)
#include <cstdio>
#include <vector>

struct Def { long MaxStamina; float StartAge; long RenownLevels; };
struct Stats {
    bool MoralityChangingEnabled; float SunTan, Fatness, Age; long Money, Stamina, MaxStamina, ExtraMaxStamina;
    float SoundRadiusMultiplier, VisibilityMultiplier, DrinkAffectMultiplier; long FrameToVomit, DeedHandle;
    bool UpdateDrunkenness, FailQuestOnDeath, IsInCutSceneFirstTime; long RenownMaxLevel, FramesUntilFatnessReduces;
    std::vector<long> ExperienceSpentOn, HeroStatsExperience, RegionTime, NumberOfTimesUsedAbility;
    float UCounter;
    Stats(const Def& def, long maps, long fatnessFrames, float random)
        : MoralityChangingEnabled(true), SunTan(0.5f), Fatness(0.0f), Money(0), Stamina(0), MaxStamina(0), ExtraMaxStamina(0),
          SoundRadiusMultiplier(1.0f), VisibilityMultiplier(1.0f), DrinkAffectMultiplier(1.0f), FrameToVomit(-1), DeedHandle(-1),
          UpdateDrunkenness(true), FailQuestOnDeath(true), IsInCutSceneFirstTime(true)
    {
        ExperienceSpentOn.resize(11, 0);
        NumberOfTimesUsedAbility.resize(20, 0);
        RegionTime.resize(maps, 0);
        HeroStatsExperience.resize(4, 0);
        UCounter = random;
        FramesUntilFatnessReduces = fatnessFrames;
        Age = def.StartAge;
        Stamina = MaxStamina = def.MaxStamina + ExtraMaxStamina;
        RenownMaxLevel = def.RenownLevels;
    }
};
int main()
{
    Def def = { 120, 18.0f, 7 };
    Stats s(def, 33, 900, 0.25f);
    if (!s.MoralityChangingEnabled || s.SunTan != 0.5f || s.Age != 18.0f || s.Stamina != 120 || s.MaxStamina != 120 ||
        s.ExperienceSpentOn.size() != 11 || s.NumberOfTimesUsedAbility.size() != 20 || s.RegionTime.size() != 33 ||
        s.HeroStatsExperience.size() != 4 || s.FrameToVomit != -1 || s.RenownMaxLevel != 7 || s.FramesUntilFatnessReduces != 900 ||
        !s.IsInCutSceneFirstTime || s.UCounter != 0.25f) { std::printf("0057cdb5_TEST FAIL\n"); return 1; }
    std::printf("0057cdb5_TEST PASS\n");
    return 0;
}