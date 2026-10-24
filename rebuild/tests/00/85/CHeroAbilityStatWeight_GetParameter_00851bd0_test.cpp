// Behaviour test for CHeroAbilityStatWeight::GetParameter (0x00851BD0): weighted mean of level/max over the weighted stats.
#include <cstdio>
#include <map>
enum EHeroTrainableStatType { STAT_A = 1, STAT_B = 2 };
const float g_epsilon_0129ba3c = 1e-4f;

class CTCHeroExperience {
public:
    long Levels[3];
    long MaxLevels[3];
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const { return Levels[stat]; }
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const { return MaxLevels[stat]; }
};

class CHeroAbilityStatWeight {
public:
    std::map<EHeroTrainableStatType, float> StatWeights;
    float GetParameter(const CTCHeroExperience& experience) const
    {
        float totalWeight = 0.0f;
        float total = 0.0f;
        for (std::map<EHeroTrainableStatType, float>::const_iterator it = StatWeights.begin(); it != StatWeights.end(); ++it) {
            EHeroTrainableStatType stat = it->first;
            float weight = it->second;
            float level = (float)experience.GetTrainableStatLevel(stat);
            total += level / experience.GetTrainableStatMaxLevel(stat) * weight;
            totalWeight += weight;
        }
        if (totalWeight < g_epsilon_0129ba3c)
            return 0.0f;
        return total / totalWeight;
    }
};

int main()
{
    CTCHeroExperience hero = { { 0, 2, 4 }, { 1, 4, 8 } };      // A at 0.5, B at 0.5
    CHeroAbilityStatWeight w;
    if (w.GetParameter(hero) != 0.0f) { std::printf("00851bd0_TEST FAIL\n"); return 1; }   // no weights
    w.StatWeights[STAT_A] = 1.0f;
    if (w.GetParameter(hero) != 0.5f) { std::printf("00851bd0_TEST FAIL\n"); return 1; }
    hero.Levels[STAT_B] = 8;                                       // B at 1.0
    w.StatWeights[STAT_B] = 3.0f;                                  // (0.5*1 + 1.0*3) / 4 = 0.875
    if (w.GetParameter(hero) != 0.875f) { std::printf("00851bd0_TEST FAIL\n"); return 1; }
    std::printf("00851bd0_TEST PASS\n");
    return 0;
}