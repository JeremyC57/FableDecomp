// Behaviour model of NScript::CQ_TentacleKrakenBossFightScript::PhaseOne (0x00DF5000): wait for the hero at the chamber entrance (10),
// objective 05, replace every DoorBlock with an invisible door + a magic barrier, Kraken intro, mother dodges while the Kraken lives, then
// remove doors and barriers (barrier-off effects), objective 06, wait at the exit trigger (6), add the Graveyard region, teleport the hero.
#include <cstdio>
#include <string>
#include <vector>
struct Sim {
    std::vector<float> HeroDistEntrance, HeroDistExit; int KrakenLifeFrames; int Frame;
    std::vector<std::string> Log; int DoorBlocks; int Live; bool MotherDodging;
};
static bool PhaseOne(Sim& g)
{
    size_t k = 0;
    while (!(g.HeroDistEntrance[k] < 10.0f)) { if (++k >= g.HeroDistEntrance.size()) return false; }
    g.Log.push_back("TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_05");
    std::vector<int> doors, barriers;
    for (int i = 0; i < g.DoorBlocks; ++i) { doors.push_back(100 + i); barriers.push_back(200 + i); g.Live += 2; }
    g.Log.push_back("CS_KRAKEN_INTRO");
    g.MotherDodging = true;
    for (int f = 0; f < g.KrakenLifeFrames; ++f) { if (!g.MotherDodging) return false; }
    for (size_t j = 0; j < doors.size(); ++j) { g.Live -= 2; g.Log.push_back("HK_MAGIC_BARRIER_OFF"); }
    doors.clear(); barriers.clear();
    g.MotherDodging = false;
    g.Log.push_back("TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06");
    k = 0;
    while (!(g.HeroDistExit[k] < 6.0f)) { if (++k >= g.HeroDistExit.size()) return false; }
    g.Log.push_back("Graveyard");
    g.Log.push_back("GraveyardFromPrisonRegionEntrance");
    return true;
}
int main()
{
    Sim g;
    g.HeroDistEntrance.push_back(30); g.HeroDistEntrance.push_back(12); g.HeroDistEntrance.push_back(9.5f);
    g.HeroDistExit.push_back(7); g.HeroDistExit.push_back(6); g.HeroDistExit.push_back(5);
    g.KrakenLifeFrames = 4; g.Frame = 0; g.DoorBlocks = 3; g.Live = 0; g.MotherDodging = false;
    if (!PhaseOne(g)) { std::printf("FAIL run\n"); return 1; }
    if (g.Log.size() != 8 || g.Log[0] != "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_05" || g.Log[1] != "CS_KRAKEN_INTRO" || g.Log[2] != "HK_MAGIC_BARRIER_OFF"
        || g.Log[5] != "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06" || g.Live != 0 || g.MotherDodging) { std::printf("FAIL log\n"); return 1; }
    Sim h = g; h.Log.clear(); h.HeroDistExit.assign(4, 6.0f);
    if (PhaseOne(h) || h.Log.back() != "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06") { std::printf("FAIL exit wait\n"); return 1; }
    std::printf("00df5000_TEST PASS\n");
    return 0;
}
