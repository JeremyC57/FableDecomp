// Behaviour test for 0x008686E0: the weighted armour threshold = sum over armour of threshold(attribute) * weight / total
// (an armour without a threshold for the attribute contributes 0; an empty weights map gives 0).
#include <cstdio>
#include <map>

class CArmourDef { public: std::map<int, float> ArmourThresholds; };
typedef std::multimap<const CArmourDef*, float> TArmourWeights;

static float GetArmourThresholdValueFromArmourWeights(int attribute, const TArmourWeights& weights, float totalWeight)
{
    float threshold = 0.0f;
    for (TArmourWeights::const_iterator it = weights.begin(); it != weights.end(); ++it)
    {
        float weight = it->second;
        float value = 0.0f;
        std::map<int, float>::const_iterator found = it->first->ArmourThresholds.find(attribute);
        if (found != it->first->ArmourThresholds.end()) value = found->second;
        threshold += value * weight / totalWeight;
    }
    return threshold;
}

int main()
{
    CArmourDef helmet, chest;
    helmet.ArmourThresholds[1] = 10.0f;
    chest.ArmourThresholds[1] = 20.0f; chest.ArmourThresholds[2] = 4.0f;
    TArmourWeights w;
    int fails = 0;
    if (GetArmourThresholdValueFromArmourWeights(1, w, 1.0f) != 0.0f) ++fails;
    w.insert(TArmourWeights::value_type(&helmet, 1.0f));
    w.insert(TArmourWeights::value_type(&chest, 3.0f));
    float t1 = GetArmourThresholdValueFromArmourWeights(1, w, 4.0f);   // 10*1/4 + 20*3/4 = 17.5
    float t2 = GetArmourThresholdValueFromArmourWeights(2, w, 4.0f);   // 0 + 4*3/4 = 3
    if (t1 < 17.49f || t1 > 17.51f || t2 < 2.99f || t2 > 3.01f) ++fails;
    if (GetArmourThresholdValueFromArmourWeights(7, w, 4.0f) != 0.0f) ++fails;
    if (fails) { std::printf("008686e0_TEST FAIL\n"); return 1; }
    std::printf("008686e0_TEST PASS\n");
    return 0;
}