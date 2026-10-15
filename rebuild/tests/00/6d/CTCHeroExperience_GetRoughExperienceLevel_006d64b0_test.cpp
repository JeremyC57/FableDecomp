#include <cstdio>

// Model of CTCHeroExperience::GetRoughExperienceLevel @ 0x006D64B0: count the thresholds the total has reached.
struct CLongVector { long* _Myfirst; long* _Mylast; unsigned long size() const { return (unsigned long)(_Mylast - _Myfirst); } };
struct CHeroExperienceDef { CLongVector ExperienceLevelThresholds; };
struct CTCHeroExperience {
    long TotalExperiencePoints; CHeroExperienceDef* PDef;
    long GetRoughExperienceLevel() const
    {
        unsigned long level;
        for (level = 0; level < PDef->ExperienceLevelThresholds.size(); ++level)
            if (TotalExperiencePoints < PDef->ExperienceLevelThresholds._Myfirst[level]) break;
        return (long)level;
    }
};

int main()
{
    long thresholds[4] = { 100, 300, 600, 1000 };
    CHeroExperienceDef def; def.ExperienceLevelThresholds._Myfirst = thresholds; def.ExperienceLevelThresholds._Mylast = thresholds + 4;
    CTCHeroExperience e; e.PDef = &def;
    e.TotalExperiencePoints = 50;   if (e.GetRoughExperienceLevel() != 0) { std::printf("006d64b0_TEST FAIL\n"); return 1; }
    e.TotalExperiencePoints = 300;  if (e.GetRoughExperienceLevel() != 2) { std::printf("006d64b0_TEST FAIL\n"); return 1; }
    e.TotalExperiencePoints = 5000; if (e.GetRoughExperienceLevel() != 4) { std::printf("006d64b0_TEST FAIL\n"); return 1; }
    def.ExperienceLevelThresholds._Mylast = thresholds;
    if (e.GetRoughExperienceLevel() != 0) { std::printf("006d64b0_TEST FAIL\n"); return 1; }
    std::printf("006d64b0_TEST PASS\n");
    return 0;
}