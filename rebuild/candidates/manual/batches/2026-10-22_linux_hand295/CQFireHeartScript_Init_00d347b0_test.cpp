#include <cstdio>
// behaviour test: CQ_FireHeartScript::Init resets the quest flags and sizes the per-prophet / per-villager tables
// from the script def; the starting sun count depends on the hero's morality (< 0.5 evil, else good)
struct SScriptDef {
    long FHNumberOfSunsEvil, FHNumberOfSunsGood, FHNumberOfPads, FHNumberOfProphets, FHNumberOfVillagers;
    float FHMoralityChangeOnKilled, FHMoralityChangeOnFreed;
};
static SScriptDef g_def;
static float g_morality = 0.2f;
struct CBoolTable { int size; bool fill; void resize(int n, bool v) { size = n; fill = v; } };
struct CQ_FireHeartScript {
    bool MissionFailed, MissionSucceeded, KilledAllPrisoners, FreedAllPrisoners, HasDemonDoorBeenEnabled;
    long StartNumberOfSuns, NumberOfPads, NumberOfProphets, NumberOfVillagers, NumberOfSuns, PuzzleRound;
    long FreedProphets, KilledProphets, EvilRounds, GoodRounds;
    float MoralityChangeOnKilled, MoralityChangeOnFreed;
    CBoolTable StartingPattern, ProphetsKilled, ShouldVillagerSpeak, CommentGivenThisRound;
    void Init();
};
void CQ_FireHeartScript::Init()
{
    MissionFailed = false; MissionSucceeded = false; HasDemonDoorBeenEnabled = false;
    if (g_morality < 0.5f) StartNumberOfSuns = g_def.FHNumberOfSunsEvil;
    else StartNumberOfSuns = g_def.FHNumberOfSunsGood;
    NumberOfPads = g_def.FHNumberOfPads;
    NumberOfSuns = 0; PuzzleRound = 0; FreedProphets = 0; KilledProphets = 0;
    NumberOfProphets = g_def.FHNumberOfProphets;
    NumberOfVillagers = g_def.FHNumberOfVillagers;
    StartingPattern.resize(NumberOfPads, false);
    ProphetsKilled.resize(NumberOfProphets, false);
    ShouldVillagerSpeak.resize(NumberOfVillagers, false);
    CommentGivenThisRound.resize(5, false);
    MoralityChangeOnKilled = g_def.FHMoralityChangeOnKilled;
    MoralityChangeOnFreed = g_def.FHMoralityChangeOnFreed;
    KilledAllPrisoners = true; FreedAllPrisoners = true; EvilRounds = 0; GoodRounds = 0;
}
int main()
{
    g_def.FHNumberOfSunsEvil = 2; g_def.FHNumberOfSunsGood = 5; g_def.FHNumberOfPads = 9;
    g_def.FHNumberOfProphets = 4; g_def.FHNumberOfVillagers = 6;
    g_def.FHMoralityChangeOnKilled = -0.25f; g_def.FHMoralityChangeOnFreed = 0.5f;
    CQ_FireHeartScript s; s.MissionFailed = true; s.KilledAllPrisoners = false; s.EvilRounds = 3;
    s.Init();
    bool ok = !s.MissionFailed && s.StartNumberOfSuns == 2 && s.StartingPattern.size == 9 && s.ProphetsKilled.size == 4
           && s.ShouldVillagerSpeak.size == 6 && s.CommentGivenThisRound.size == 5 && !s.StartingPattern.fill
           && s.KilledAllPrisoners && s.FreedAllPrisoners && s.EvilRounds == 0 && s.MoralityChangeOnFreed == 0.5f;
    g_morality = 0.75f;
    s.Init();
    ok = ok && s.StartNumberOfSuns == 5;
    if (!ok) { std::printf("00d347b0_TEST FAIL\n"); return 1; }
    std::printf("00d347b0_TEST PASS\n");
    return 0;
}
