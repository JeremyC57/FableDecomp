// Body fixture: selected function and original clamp template pasted unchanged.
// The script base services are not used here; data fields use native host widths.
#include <cassert>
#include <cstdio>
#include <cstring>
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RandomPopulationSimScript {
public:
    enum POPULATION_TYPES { PT_VILLAGERS, PT_GUARDS, PT_BANDITS, PT_MAX };
    class CPopulationSim { public: long MaxPopulation; };
    float CurrentPopulationScale[4];
    bool HighDetailSimulationActive;
    long CurrentPopulationRegion;
    long CreaturesAlive[3];
    unsigned long BanditCreatureGeneratorIndex;
    unsigned long VillagerCreatureGeneratorIndex;
    unsigned long GuardCreatureGeneratorIndex;
    CPopulationSim PopulationSim[3];
    long GetCurrentPopulationLevel(POPULATION_TYPES population);
};
}
template<class T, class L, class U>
inline void GFLimit(T& value, const L& lower, const U& upper)
{
    if (value < lower) value = lower;
    else if (value > upper) value = upper;
}
namespace NScript {
long CV_RandomPopulationSimScript::GetCurrentPopulationLevel(POPULATION_TYPES population)
{
    float population_scale = 0.0f;
    switch (population) {
    case PT_VILLAGERS:
        population_scale = 1.0f - CurrentPopulationScale[CurrentPopulationRegion];
        break;
    case PT_GUARDS:
        population_scale = CurrentPopulationScale[CurrentPopulationRegion];
        break;
    case PT_BANDITS:
        population_scale = 0.0f;
        break;
    }
    GFLimit(population_scale, 0, 1);
    return (long)(PopulationSim[population].MaxPopulation * population_scale);
}
}

int main()
{
    typedef NScript::CV_RandomPopulationSimScript Script;
    const float scales[] = {-2.0f, -0.25f, 0.0f, 0.125f, 0.5f, 0.875f, 1.0f, 1.25f, 2.0f};
    const long maxima[] = {-17, -1, 0, 1, 7, 16, 123};
    int count = 0;
    for (int region = 0; region != 4; ++region)
        for (int scale = 0; scale != 9; ++scale)
            for (int maximum = 0; maximum != 7; ++maximum)
                for (int kind = 0; kind != 3; ++kind) {
                    Script script;
                    std::memset(&script, 0, sizeof(script));
                    script.CurrentPopulationRegion = region;
                    for (int i = 0; i != 4; ++i) script.CurrentPopulationScale[i] = 0.25f;
                    script.CurrentPopulationScale[region] = scales[scale];
                    for (int i = 0; i != 3; ++i) script.PopulationSim[i].MaxPopulation = maxima[maximum] + i;
                    Script saved = script;
                    float fraction = kind == Script::PT_BANDITS ? 0.0f :
                        kind == Script::PT_GUARDS ? scales[scale] : 1.0f - scales[scale];
                    if (fraction < 0.0f) fraction = 0.0f;
                    if (fraction > 1.0f) fraction = 1.0f;
                    long expected = (long)((maxima[maximum] + kind) * fraction);
                    assert(script.GetCurrentPopulationLevel((Script::POPULATION_TYPES)kind) == expected);
                    assert(std::memcmp(&script, &saved, sizeof(script)) == 0);
                    ++count;
                }
    assert(count == 756);
    std::puts("00ec3270_TEST PASS");
}