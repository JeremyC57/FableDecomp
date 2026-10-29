// Behaviour test for 0x00DA24F0 NScript::CQ_MinionCampScript::RunChantTimer: the chant timer starts at 72 and counts down one
// per frame; while ChantTimerPaused it is held at PausedTime (and set once more when the pause ends); every frame the bar
// shows 72 - time; the loop ends when the time is negative and the timer is not paused.
#include <cstdio>

class CGameScriptInterface { public:
    long Timers[4]; int Frames; int PauseFrom, PauseTo; bool* Paused; int BarUpdates; float LastBar; long LastBarId;
    void SetTimer(long t, long v) { Timers[t] = v; }
    long GetTimer(long t) const { return Timers[t]; }
    void NewScriptFrame()
    {
        ++Frames; --Timers[1];
        *Paused = Frames >= PauseFrom && Frames < PauseTo;
    }
    void UpdateQuestInfoBar(long id, float v, float a, float b) { ++BarUpdates; LastBar = v; LastBarId = id; if (a != -1.0f || b != -1.0f) BarUpdates = -1000; }
};
static CGameScriptInterface g_game;
static CGameScriptInterface* g_pGameScriptInterface = &g_game;
class CTimer { public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    bool operator>=(long time) const { return GetTime() >= time; }
};
struct CQ_MinionCampScript {
    CGameScriptInterface* Game; CTimer ChantTimer; long ChantTimerBar; bool ChantTimerPaused; long PausedTime;
    int TermAt, TermCalls;
    bool IsActiveThreadTerminating() { return TermAt && ++TermCalls >= TermAt; }
    void RunChantTimer()
    {
        ChantTimer = 72;
        bool wasPaused = false;
        while (ChantTimer >= 0 || ChantTimerPaused) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (!ChantTimerPaused) {
                if (IsActiveThreadTerminating())
                    return;
                if (wasPaused) {
                    if (IsActiveThreadTerminating())
                        return;
                    ChantTimer = PausedTime;
                    wasPaused = false;
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                ChantTimer = PausedTime;
                wasPaused = true;
            }
            Game->UpdateQuestInfoBar(ChantTimerBar, (float)(72 - ChantTimer.GetTime()), -1.0f, -1.0f);
        }
        if (IsActiveThreadTerminating())
            return;
    }
};

int main()
{
    CQ_MinionCampScript s; s.Game = &g_game; s.ChantTimer.TimerIndex = 1; s.ChantTimerBar = 9; s.ChantTimerPaused = false;
    s.PausedTime = 50; s.TermAt = 0; s.TermCalls = 0;
    g_game.Frames = 0; g_game.PauseFrom = 1000; g_game.PauseTo = 0; g_game.Paused = &s.ChantTimerPaused; g_game.BarUpdates = 0;
    s.RunChantTimer();   // no pause: 73 frames (72 .. -1)
    int ok = g_game.Frames == 73 && g_game.BarUpdates == 73 && g_game.LastBar == 73.0f && g_game.LastBarId == 9;
    // paused for frames 10..19: the timer is held at 50 while paused and reset to 50 once after the pause
    g_game.Frames = 0; g_game.BarUpdates = 0; g_game.PauseFrom = 10; g_game.PauseTo = 20;
    s.RunChantTimer();
    ok = ok && g_game.Frames == 20 + 51 && g_game.BarUpdates == g_game.Frames;
    // terminating: only the first frame runs
    g_game.Frames = 0; g_game.BarUpdates = 0; g_game.PauseFrom = 1000; s.TermAt = 1; s.TermCalls = 0;
    s.RunChantTimer();
    ok = ok && g_game.Frames == 1 && g_game.BarUpdates == 0;
    if (!ok) { std::printf("00da24f0_TEST FAIL %d %d\n", g_game.Frames, g_game.BarUpdates); return 1; }
    std::printf("00da24f0_TEST PASS\n");
    return 0;
}