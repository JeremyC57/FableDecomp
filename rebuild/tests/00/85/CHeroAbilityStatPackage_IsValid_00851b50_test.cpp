// Behaviour test for CHeroAbilityStatPackage::IsValid (0x00851B50): every stat threshold must be reached.
#include <cstdio>
#include <map>
enum EHeroTrainableStatType { STAT_A = 1, STAT_B = 2, STAT_C = 3 };

class CTCHeroExperience {
public:
    long Levels[4];
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const { return Levels[stat]; }
};

class CHeroAbilityStatPackage {
public:
    std::map<EHeroTrainableStatType, long> StatThresholds;
    bool IsValid(const CTCHeroExperience& experience) const
    {
        for (std::map<EHeroTrainableStatType, long>::const_iterator it = StatThresholds.begin(); it != StatThresholds.end(); ++it) {
            long threshold = it->second;
            if (experience.GetTrainableStatLevel(it->first) < threshold)
                return false;
        }
        return true;
    }
};

int main()
{
    CTCHeroExperience hero = { { 0, 3, 5, 1 } };
    CHeroAbilityStatPackage p;
    if (!p.IsValid(hero)) { std::printf("00851b50_TEST FAIL\n"); return 1; }        // no thresholds
    p.StatThresholds[STAT_A] = 3;
    p.StatThresholds[STAT_B] = 4;
    if (!p.IsValid(hero)) { std::printf("00851b50_TEST FAIL\n"); return 1; }        // 3>=3, 5>=4
    p.StatThresholds[STAT_C] = 2;
    if (p.IsValid(hero)) { std::printf("00851b50_TEST FAIL\n"); return 1; }         // 1 < 2
    std::printf("00851b50_TEST PASS\n");
    return 0;
}