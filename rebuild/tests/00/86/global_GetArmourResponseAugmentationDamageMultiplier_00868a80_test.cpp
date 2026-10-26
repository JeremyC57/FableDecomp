// Behaviour test for 0x00868A80: multiplier = sum over armour of response * weight / total * multiplier,
// response 1 when the armour has none; always true (an empty weights map stores 0).
#include <cstdio>
#include <map>

class CArmourDef { public: std::map<int, float> AugmentationResponse; };
typedef std::multimap<const CArmourDef*, float> TArmourWeights;

static bool GetMultiplier(int attribute, const TArmourWeights& weights, float totalWeight, float& multiplier)
{
    float result = 0.0f;
    for (TArmourWeights::const_iterator it = weights.begin(); it != weights.end(); ++it)
    {
        float scale = it->second / totalWeight * multiplier;
        float response = 1.0f;
        std::map<int, float>::const_iterator found = it->first->AugmentationResponse.find(attribute);
        if (found != it->first->AugmentationResponse.end()) response = found->second;
        result += response * scale;
    }
    multiplier = result;
    return true;
}

int main()
{
    CArmourDef a, b;
    a.AugmentationResponse[1] = 0.5f;
    TArmourWeights w;
    int fails = 0;
    float m = 2.0f;
    if (!GetMultiplier(1, w, 1.0f, m) || m != 0.0f) ++fails;
    w.insert(TArmourWeights::value_type(&a, 1.0f));
    w.insert(TArmourWeights::value_type(&b, 1.0f));
    m = 2.0f;
    if (!GetMultiplier(1, w, 2.0f, m)) ++fails;          // 0.5*1/2*2 + 1*1/2*2 = 1.5
    if (m < 1.49f || m > 1.51f) ++fails;
    m = 2.0f;
    GetMultiplier(9, w, 2.0f, m);                        // no responses: unchanged 2
    if (m < 1.99f || m > 2.01f) ++fails;
    if (fails) { std::printf("00868a80_TEST FAIL\n"); return 1; }
    std::printf("00868a80_TEST PASS\n");
    return 0;
}