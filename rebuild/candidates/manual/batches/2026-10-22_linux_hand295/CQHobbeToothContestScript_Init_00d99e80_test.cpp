#include <cstdio>
#include <cstring>
// behaviour test: CQ_HobbeToothContestScript::Init -- register OrchardFarm, reset the contest (8 teeth needed,
// counters zero, drinking hobbes untouched), clear the master-data boast flag, register the four boasts (the last one
// KILLMOREHOBBES, id 0x1c) and zero the Whisper comment timer
struct SScriptDef { long Cost[4]; long Reward[4]; };
static SScriptDef g_def;
static int g_regions = 0, g_boasts = 0, g_lastId = 0; static long g_costSum = 0;
struct SMasterData { bool HobbeContestKillMoreHobbes; };
struct CTimer { long Value; void Set(long v) { Value = v; } };
struct CGameScriptInterface {
    void AddQuestRegion(const char* q, const char* r) const { if (!std::strcmp(q, "Q_HobbeToothContest") && !std::strcmp(r, "OrchardFarm")) ++g_regions; }
    void AddBoast(const char*, long id, long cost, long) const { ++g_boasts; g_lastId = id; g_costSum += cost; }
};
struct CQ_HobbeToothContestScript {
    CGameScriptInterface* Game; SMasterData* PMasterData; CTimer WhisperCommentTimer;
    long NumberOfHobbeTeethNeeded, NumberOfHobbeTeethCollectedByHero, NumberOfHobbeTeethCollectedByWhisper;
    bool DrinkingHobbesNotHit, MissionSucceeded, MissionFailed;
    void Init();
};
void CQ_HobbeToothContestScript::Init()
{
    Game->AddQuestRegion("Q_HobbeToothContest", "OrchardFarm");
    NumberOfHobbeTeethCollectedByHero = 0; NumberOfHobbeTeethCollectedByWhisper = 0; NumberOfHobbeTeethNeeded = 8;
    DrinkingHobbesNotHit = true; MissionSucceeded = false; MissionFailed = false;
    PMasterData->HobbeContestKillMoreHobbes = false;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_def.Cost[0], g_def.Reward[0]);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_def.Cost[1], g_def.Reward[1]);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_def.Cost[2], g_def.Reward[2]);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLMOREHOBBES", 0x1c, g_def.Cost[3], g_def.Reward[3]);
    WhisperCommentTimer.Set(0);
}
int main()
{
    for (int i = 0; i < 4; ++i) { g_def.Cost[i] = i + 1; g_def.Reward[i] = 0; }
    CGameScriptInterface game; SMasterData md; md.HobbeContestKillMoreHobbes = true;
    CQ_HobbeToothContestScript s; s.Game = &game; s.PMasterData = &md; s.WhisperCommentTimer.Value = 9;
    s.NumberOfHobbeTeethCollectedByHero = 5; s.DrinkingHobbesNotHit = false; s.MissionFailed = true;
    s.Init();
    bool ok = g_regions == 1 && g_boasts == 4 && g_lastId == 0x1c && g_costSum == 10 && !md.HobbeContestKillMoreHobbes
           && s.WhisperCommentTimer.Value == 0 && s.NumberOfHobbeTeethNeeded == 8 && s.NumberOfHobbeTeethCollectedByHero == 0
           && s.DrinkingHobbesNotHit && !s.MissionFailed;
    if (!ok) { std::printf("00d99e80_TEST FAIL\n"); return 1; }
    std::printf("00d99e80_TEST PASS\n");
    return 0;
}
