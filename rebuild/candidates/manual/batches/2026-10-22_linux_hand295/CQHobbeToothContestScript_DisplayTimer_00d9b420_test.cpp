#include <cstdio>
#include <cstring>
// behaviour test: CQ_HobbeToothContestScript::DisplayTimer -- run the contest clock with the hero / Whisper tooth
// counters; while the hero is out of OrchardFarmEast, Whisper scores a tooth every 5 seconds (the clock restarts each
// time he leaves); when the time is up, wait for him to be back and remove the three HUD elements
static int g_frames = 0, g_endAt = 1000, g_removed = 0; static bool g_shown = false;
static const char g_inArea[] = "11000000000000111111";  // OrchardFarmEast loaded per frame
struct CTimer { float Time; void SetTime(float t) { Time = t; } float GetTime() const { return Time; } };
static CTimer* g_timers[2]; static int g_timerCount = 0;
struct SScriptDef { float HOB_TotalTime; };
static SScriptDef g_def;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; for (int i = 0; i < g_timerCount; ++i) g_timers[i]->Time -= 1.0f; }
    bool IsLevelLoaded(const char* n) const { return !std::strcmp(n, "OrchardFarmEast") && g_inArea[g_frames] == '1'; }
    void DisplayQuestInfo(bool d) const { g_shown = d; }
    long AddQuestInfoTimer(CTimer& t) const { g_timers[g_timerCount++] = &t; return 1; }
    long AddQuestInfoCounter(const char* icon) const { return !std::strcmp(icon, "HUD_HERO_ICON") ? 2 : 3; }
    void UpdateQuestInfoCounter(long, long) const { }
    void RemoveQuestInfoElement(long id) const { if (id >= 1 && id <= 3) ++g_removed; }
};
struct CQ_HobbeToothContestScript {
    CGameScriptInterface* Game; long NumberOfHobbeTeethCollectedByHero, NumberOfHobbeTeethCollectedByWhisper;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void DisplayTimer();
};
void CQ_HobbeToothContestScript::DisplayTimer()
{
    CTimer contestTimer;
    contestTimer.SetTime((float)(long)g_def.HOB_TotalTime);
    Game->DisplayQuestInfo(true);
    long timerElement = Game->AddQuestInfoTimer(contestTimer);
    long heroElement = Game->AddQuestInfoCounter("HUD_HERO_ICON");
    long whisperElement = Game->AddQuestInfoCounter("HUD_WHISPER_ICON");
    CTimer whisperTimer; g_timers[g_timerCount++] = &whisperTimer;
    whisperTimer.SetTime(10);
    bool heroInArea = true;
    while (contestTimer.GetTime() > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->UpdateQuestInfoCounter(heroElement, NumberOfHobbeTeethCollectedByHero);
        Game->UpdateQuestInfoCounter(whisperElement, NumberOfHobbeTeethCollectedByWhisper);
        if (!Game->IsLevelLoaded("OrchardFarmEast")) {
            if (heroInArea) { whisperTimer.SetTime(5); heroInArea = false; }
            if (whisperTimer.GetTime() <= 0) { ++NumberOfHobbeTeethCollectedByWhisper; whisperTimer.SetTime(5); }
        } else if (!heroInArea) heroInArea = true;
    }
    if (IsActiveThreadTerminating()) return;
    while (!Game->IsLevelLoaded("OrchardFarmEast")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveQuestInfoElement(timerElement);
    Game->RemoveQuestInfoElement(whisperElement);
    Game->RemoveQuestInfoElement(heroElement);
    Game->DisplayQuestInfo(false);
}
int main()
{
    g_def.HOB_TotalTime = 12.9f;                        // truncated to 12 seconds
    CGameScriptInterface game; CQ_HobbeToothContestScript s; s.Game = &game;
    s.NumberOfHobbeTeethCollectedByHero = 0; s.NumberOfHobbeTeethCollectedByWhisper = 0;
    s.DisplayTimer();                                   // out of the area from frame 2: Whisper scores at frames 7 and 12
    bool ok = g_frames == 14 && s.NumberOfHobbeTeethCollectedByWhisper == 2 && g_removed == 3 && !g_shown;
    g_frames = 0; g_timerCount = 0; g_removed = 0; g_endAt = 13;
    s.DisplayTimer();                                   // thread ends while waiting for the hero: HUD left up
    ok = ok && g_removed == 0 && g_shown;
    if (!ok) { std::printf("00d9b420_TEST FAIL\n"); return 1; }
    std::printf("00d9b420_TEST PASS\n");
    return 0;
}
