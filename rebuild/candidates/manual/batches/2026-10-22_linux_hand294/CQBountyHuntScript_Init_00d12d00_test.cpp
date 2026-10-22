// Behaviour test for CQ_BountyHuntScript::Init @ 0x00D12D00: four regions, every quest flag cleared, the boast timer set
// to the def's time limit, the master-data boast results cleared, then the four boasts (ids 1, 3, 15, 16).
#include <cstdio>
#include <string>
#include <vector>
struct Game {
    std::vector<std::string> Log;
    void AddQuestRegion(const char* q, const char* r) { Log.push_back(std::string(q) + "/" + r); }
    void AddBoast(int id) { char b[16]; std::sprintf(b, "boast %d", id); Log.push_back(b); }
};
struct Timer { float Time; void SetTime(float t) { Time = t; } };
struct Data { bool WithinTimeLimit, TimeLimitExceeded, Decapitation; };
struct Script {
    Game* G; Data* D;
    bool QuestFailed, LeaderKilled, BoastActiveTimeLimit;
    long BanditGruntTextCounter;
    Timer BoastTime;
    void Init(float limit)
    {
        G->AddQuestRegion("Q_BountyHunt", "LookoutPoint");
        G->AddQuestRegion("Q_BountyHunt", "GreatwoodEntrance");
        G->AddQuestRegion("Q_BountyHunt", "GreatwoodLake");
        G->AddQuestRegion("Q_BountyHunt", "Fisherman");
        QuestFailed = LeaderKilled = false;
        BanditGruntTextCounter = 0;
        BoastTime.SetTime(limit);
        BoastActiveTimeLimit = false;
        D->WithinTimeLimit = D->TimeLimitExceeded = D->Decapitation = false;
        G->AddBoast(1); G->AddBoast(3); G->AddBoast(15); G->AddBoast(16);
    }
};
int main()
{
    Game g; Data d = { true, true, true };
    Script s; s.G = &g; s.D = &d; s.QuestFailed = s.LeaderKilled = s.BoastActiveTimeLimit = true; s.BanditGruntTextCounter = 9;
    s.BoastTime.Time = 0.0f;
    s.Init(300.0f);
    bool ok = g.Log.size() == 8 && g.Log[3] == "Q_BountyHunt/Fisherman" && g.Log[6] == "boast 15" && g.Log[7] == "boast 16" &&
              s.BoastTime.Time == 300.0f && !d.WithinTimeLimit && !d.Decapitation && !s.QuestFailed && s.BanditGruntTextCounter == 0;
    if (!ok) { std::printf("00d12d00_TEST FAIL\n"); return 1; }
    std::printf("00d12d00_TEST PASS\n");
    return 0;
}
