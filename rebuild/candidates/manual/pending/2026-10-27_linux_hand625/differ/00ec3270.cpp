// CV_RandomPopulationSimScript::GetCurrentPopulationLevel(long type) -- the current population target for a creature type: the region's population scale (inverted for type 0, as is for type 1, none for type 2), clamped to [0, 1], times the type's maximum population, truncated
// Grade: DIFFER(123v123) -- one jump target: retail's switch default (level still the initial 0.0f) jumps to the `> 1.0f` clamp test (+0x56), ours threads it past both clamp tests to the final multiply (+0x6b); 2 masked lines
// Identity: FableWin.pdb ?GetCurrentPopulationLevel@CV_RandomPopulationSimScript@NScript@@QAEJW4POPULATION_TYPES@12@@Z -- NScript::CV_RandomPopulationSimScript::GetCurrentPopulationLevel(NScript::CV_RandomPopulationSimScript::POPULATION_TYPES); respell at landing (namespace NScript as in the landed sibling ProcessAlbionPopulationSim 0x00EC2240, nested enum parameter).
// Leaf: GetCurrentPopulationLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC3270, length 122 bytes (padded to 128), `ret 4`.
// Callees: __ftol2 @ 0x00BFEA70. Constants: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout (retail = Ego_r.pdb CV_RandomPopulationSimScript - 0x14): CurrentPopulationScale[4] +0x48,
// HighDetailSimulationActive +0x58, CurrentPopulationRegion +0x5c, CreaturesAlive[3] +0x60, the three generator
// indices +0x6c, PopulationSim[3] (CPopulationSim {MaxPopulation}, 4 bytes) +0x78.
// Shape: `level` starts at 0.0f (loaded before the switch); case 2 is threaded into the clamp's zero assignment.
struct CPopulationSim {
    long MaxPopulation;
};
struct CV_RandomPopulationSimScript {
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    float CurrentPopulationScale[4];               // +0x48
    bool  HighDetailSimulationActive;              // +0x58
    long  CurrentPopulationRegion;                 // +0x5c
    long  CreaturesAlive[3];                       // +0x60
    unsigned long BanditCreatureGeneratorIndex;    // +0x6c
    unsigned long VillagerCreatureGeneratorIndex;  // +0x70
    unsigned long GuardCreatureGeneratorIndex;     // +0x74
    CPopulationSim PopulationSim[3];               // +0x78
    long GetCurrentPopulationLevel(long type);
};
long CV_RandomPopulationSimScript::GetCurrentPopulationLevel(long type)
{
    float level = 0.0f;
    switch (type)
    {
    case 0:
        level = 1.0f - CurrentPopulationScale[CurrentPopulationRegion];
        break;
    case 1:
        level = CurrentPopulationScale[CurrentPopulationRegion];
        break;
    case 2:
        level = 0.0f;
        break;
    }
    if (level < 0.0f)
        level = 0.0f;
    if (level > 1.0f)
        level = 1.0f;
    return (long)(PopulationSim[type].MaxPopulation * level);
}
