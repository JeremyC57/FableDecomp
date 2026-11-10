// NScript::CV_RandomPopulationSimScript::GetCurrentPopulationLevel(POPULATION_TYPES) -- compute the clamped population target.
// Leaf: ?GetCurrentPopulationLevel@CV_RandomPopulationSimScript@NScript@@QAEJW4POPULATION_TYPES@12@@Z
// Retail: 0x00ec3270, 123 bytes, thiscall.
#define FABLE_RETAIL_NSCRIPT
#include "engine/NScript_CV_RandomPopulationSimScript.h"
#include "engine/GFLimit.h"
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