// Behaviour test for CQ_ArenaScript::Init @ 0x00CFA700: four quest regions, four boasts (ids 1, 3, 30, 31 with the
// script-def costs/rewards), the arena state reset, and the spawn-needed flags cleared unless the thread is terminating.
#include <cstdio>
#include <string>
#include <vector>
struct Game {
    std::vector<std::string> Log;
    void AddQuestRegion(const char* q, const char* r) { Log.push_back(std::string(q) + "/" + r); }
    void AddBoast(const char* text, int id, int cost, int reward, bool a, const char* s, int b)
    { (void)text; (void)s; char buf[64]; std::sprintf(buf, "boast %d %d %d %d %d", id, cost, reward, a ? 1 : 0, b); Log.push_back(buf); }
};
struct Arena {
    Game* G;
    int TerminateAfter, Polls;
    int ArenaRound, NewCrowdPoints, ArenaState;
    bool PauseCrowdChecker, WhisperNeededForCutscene, ArenaSpawnNeeded[16];
    bool IsActiveThreadTerminating() { return ++Polls > TerminateAfter; }
    void Init()
    {
        G->AddQuestRegion("Q_Arena", "ArenaExterior");
        G->AddQuestRegion("Q_Arena", "ArenaHallOfHeroes");
        G->AddQuestRegion("Q_Arena", "ArenaCells");
        G->AddQuestRegion("Q_Arena", "Arena");
        G->AddBoast("NAKED", 1, 10, 11, false, "", 0);
        G->AddBoast("NODAMAGE", 3, 20, 21, false, "", 0);
        G->AddBoast("KILL", 30, 30, 31, true, "", 1);
        G->AddBoast("SPARE", 31, 40, 41, false, "", 1);
        ArenaRound = 0;
        NewCrowdPoints = 200;
        PauseCrowdChecker = true;
        for (int i = 0; i < 16; ++i) {
            if (IsActiveThreadTerminating()) return;
            ArenaSpawnNeeded[i] = false;
        }
        if (IsActiveThreadTerminating()) return;
        ArenaState = 0;
        WhisperNeededForCutscene = true;
    }
};
int main()
{
    Game g;
    Arena a;
    a.G = &g; a.TerminateAfter = 100; a.Polls = 0; a.ArenaRound = 5; a.ArenaState = 9; a.WhisperNeededForCutscene = false;
    for (int i = 0; i < 16; ++i) a.ArenaSpawnNeeded[i] = true;
    a.Init();
    bool ok = g.Log.size() == 8 && g.Log[0] == "Q_Arena/ArenaExterior" && g.Log[3] == "Q_Arena/Arena" &&
              g.Log[6] == "boast 30 30 31 1 1" && g.Log[7] == "boast 31 40 41 0 1" && a.ArenaRound == 0 &&
              a.NewCrowdPoints == 200 && a.ArenaState == 0 && a.WhisperNeededForCutscene && !a.ArenaSpawnNeeded[15] && a.Polls == 17;
    Arena b = a;
    b.Polls = 0; b.TerminateAfter = 3; b.ArenaState = 9; b.WhisperNeededForCutscene = false;
    for (int i = 0; i < 16; ++i) b.ArenaSpawnNeeded[i] = true;
    b.Init();
    ok = ok && !b.ArenaSpawnNeeded[2] && b.ArenaSpawnNeeded[3] && b.ArenaState == 9 && !b.WhisperNeededForCutscene;
    if (!ok) { std::printf("00cfa700_TEST FAIL\n"); return 1; }
    std::printf("00cfa700_TEST PASS\n");
    return 0;
}
