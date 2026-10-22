// Behaviour test for CQ_BreakSiegeScript::Init @ 0x00D1CC40: two quest regions, flags cleared, ReasonForFailure 0,
// master data NoAssistance set, KilledLeader cleared, then five boasts (ids 1, 3, 6, 25, 26).
#include <cstdio>
#include <string>
#include <vector>
struct Game {
    std::vector<std::string> Log;
    void AddQuestRegion(const char* r) { Log.push_back(r); }
    void AddBoast(int id) { char b[16]; std::sprintf(b, "boast %d", id); Log.push_back(b); }
};
struct Data { bool NoAssistance, KilledLeader; };
struct Script {
    Game* G; Data* D; bool QuestFailed, QuestCompleted; int ReasonForFailure;
    void Init()
    {
        G->AddQuestRegion("KnotholeGlade"); G->AddQuestRegion("DemonDoor_KnotholeGlade");
        QuestFailed = QuestCompleted = false; ReasonForFailure = 0;
        D->NoAssistance = true; D->KilledLeader = false;
        int ids[] = { 1, 3, 6, 25, 26 };
        for (int i = 0; i < 5; ++i) G->AddBoast(ids[i]);
    }
};
int main()
{
    Game g; Data d = { false, true }; Script s = { &g, &d, true, true, 7 };
    s.Init();
    bool ok = g.Log.size() == 7 && g.Log[1] == "DemonDoor_KnotholeGlade" && g.Log[4] == "boast 6" && g.Log[6] == "boast 26" &&
              d.NoAssistance && !d.KilledLeader && s.ReasonForFailure == 0 && !s.QuestFailed;
    if (!ok) { std::printf("00d1cc40_TEST FAIL\n"); return 1; }
    std::printf("00d1cc40_TEST PASS\n");
    return 0;
}
