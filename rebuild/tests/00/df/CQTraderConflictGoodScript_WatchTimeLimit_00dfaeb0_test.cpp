// Behaviour test for CQ_TraderConflictGoodScript::WatchTimeLimit (00dfaeb0): waits for the data's start flag (or the 5-second timer), then
// re-arms the timer with the def's limit, shows it as a quest-info timer, waits for success, removes the element,
// and marks the data when time remained; the timer is deregistered exactly once on every path.
#include <cstdio>
#include <cstring>
struct CCharString {
    char Text[32];
    CCharString(const char* text) { std::strncpy(Text, text, 31); Text[31] = 0; }
};
struct CTimer;
struct CQ_TraderConflictData { bool TimeLimitBeaten; bool StartTimeLimit; };
struct CGameScriptInterface {
    int frames, registered, deregistered, displayed, removed, startAfter, timeLeft, setLimit;
    CQ_TraderConflictData* data; bool* succeeded; int succeedAfter; char icon[32];
    void NewScriptFrame() { ++frames; if (frames == startAfter) data->StartTimeLimit = true; if (frames == succeedAfter) *succeeded = true; }
    long RegisterTimer() { ++registered; return 42; }
    void DeregisterTimer(long timer) { if (timer == 42) ++deregistered; }
    void SetTimer(long timer, long seconds) { if (timer == 42) setLimit = (int)seconds; }
    long GetTimer(long timer) { return timer == 42 ? timeLeft : -1; }
    void DisplayQuestInfo(bool display) { if (display) ++displayed; }
    long AddQuestInfoTimer(const CTimer& timer, const CCharString& icon, float scale);
    void RemoveQuestInfoElement(long element) { if (element == 7) ++removed; }
};
static CGameScriptInterface g_game;
static CGameScriptInterface* g_pGameScriptInterface = &g_game;
struct CTimer {
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};
long CGameScriptInterface::AddQuestInfoTimer(const CTimer& timer, const CCharString& text, float scale)
{
    std::strcpy(icon, text.Text);
    return (timer.Index == 42 && scale == 1.0f) ? 7 : 0;
}
struct CScriptDef { long TraderConflictTimeLimit; };
static CScriptDef g_def = { 300 };
static CScriptDef* g_pScriptDef = &g_def;
struct CScript {
    CGameScriptInterface* Game; CQ_TraderConflictData* Data; bool MissionSucceeded;
    bool IsActiveThreadTerminating() { return false; }
    void WatchTimeLimit();
};
void CScript::WatchTimeLimit()
{
    CTimer timer;
    timer.SetTime(5);
    while (!Data->StartTimeLimit) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            return;
        }
    }
    timer.SetTime(g_pScriptDef->TraderConflictTimeLimit);
    long element = Game->AddQuestInfoTimer(timer, "HUD_CLOCK_ICON", 1.0f);
    Game->DisplayQuestInfo(true);
    while (!MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(element);
    if (timer.GetTime() > 0) {
        if (IsActiveThreadTerminating())
            return;
        Data->TimeLimitBeaten = true;
    }
}
static bool Run(int startAfter, int timeLeft, bool expectBeaten, bool expectShown)
{
    CQ_TraderConflictData data = { false, false };
    CScript s; s.Game = &g_game; s.Data = &data; s.MissionSucceeded = false;
    std::memset(&g_game, 0, sizeof(g_game));
    g_game.data = &data; g_game.succeeded = &s.MissionSucceeded; g_game.startAfter = startAfter;
    g_game.succeedAfter = startAfter + 3; g_game.timeLeft = timeLeft;
    s.WatchTimeLimit();
    if (g_game.registered != 1 || g_game.deregistered != 1) return false;
    if (data.TimeLimitBeaten != expectBeaten) return false;
    if (expectShown)
        return g_game.displayed == 1 && g_game.removed == 1 && g_game.setLimit == 300 && std::strcmp(g_game.icon, "HUD_CLOCK_ICON") == 0;
    return g_game.displayed == 0 && g_game.removed == 0 && g_game.setLimit == 5;
}
int main()
{
    if (!Run(2, 10, true, true) || !Run(2, 0, false, false) || !Run(2, -1, false, true)) {
        std::printf("00dfaeb0_TEST FAIL\n");
        return 1;
    }
    std::printf("00dfaeb0_TEST PASS\n");
    return 0;
}