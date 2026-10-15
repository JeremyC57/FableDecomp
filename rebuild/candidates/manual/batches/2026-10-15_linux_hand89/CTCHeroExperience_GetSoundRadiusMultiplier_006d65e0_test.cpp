#include <cstdio>

// Model of CTCHeroExperience::GetSoundRadiusMultiplier @ 0x006d65e0: table[min(stealth level, size - 1)] from the stats def.
template<class T> inline const T& Min(const T& left, const T& right) { return (right < left) ? right : left; }
struct CFloatVector { float* _Myfirst; float* _Mylast; long size() const { return (long)(_Mylast - _Myfirst); } };
struct CHeroStatsDef { CFloatVector SoundDistanceMultiplierPerStealthLevel; };
struct CTCHeroExperience {
    long TrainableStatLevels[10]; CHeroStatsDef* PHeroStatsDef;
    float GetSoundRadiusMultiplier() const
    {
        return PHeroStatsDef->SoundDistanceMultiplierPerStealthLevel._Myfirst[Min(TrainableStatLevels[5], PHeroStatsDef->SoundDistanceMultiplierPerStealthLevel.size() - 1)];
    }
};

int main()
{
    float table[3] = { 1.0f, 0.75f, 0.5f };
    CHeroStatsDef def; def.SoundDistanceMultiplierPerStealthLevel._Myfirst = table; def.SoundDistanceMultiplierPerStealthLevel._Mylast = table + 3;
    CTCHeroExperience e; for (int i = 0; i < 10; ++i) e.TrainableStatLevels[i] = 0; e.PHeroStatsDef = &def;
    e.TrainableStatLevels[5] = 1; if (e.GetSoundRadiusMultiplier() != 0.75f) { std::printf("006d65e0_TEST FAIL\n"); return 1; }
    e.TrainableStatLevels[5] = 9; if (e.GetSoundRadiusMultiplier() != 0.5f) { std::printf("006d65e0_TEST FAIL\n"); return 1; }
    e.TrainableStatLevels[4] = 2; e.TrainableStatLevels[5] = 0; if (e.GetSoundRadiusMultiplier() != 1.0f) { std::printf("006d65e0_TEST FAIL\n"); return 1; }
    std::printf("006d65e0_TEST PASS\n");
    return 0;
}
