#ifndef FABLE_ENGINE_NSCRIPT_CV_RANDOMPOPULATIONSIMSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CV_RANDOMPOPULATIONSIMSCRIPT_H
// Retail opt-in (Codex pass 17): both PDBs prove names and nested types.
// Layout: Ego_r base 0x5c becomes retail 0x48; fields retain their relative offsets.
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RandomPopulationSimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
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
#endif
