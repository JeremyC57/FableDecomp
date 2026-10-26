// Behaviour test for CQ_MinionCampScript::PauseChantTimer() @ 0x00DA2490
#include <cstdio>

struct CThingManager_Calls {
    long m_timerValue;
    virtual long GetTimerValue(long timer) { return m_timerValue + timer; }
};
static CThingManager_Calls g_thingManager;
static CThingManager_Calls* g_pThingManager_0143e8f8 = &g_thingManager;

struct CQ_MinionCampScript;
struct CGameScriptInterface_Calls {
    CQ_MinionCampScript* m_pScript;
    int m_frames;
    virtual void NewScriptFrame();
};

struct CQ_MinionCampScript {
    CGameScriptInterface_Calls* Game;
    long ChantTimer;
    long ChantTimerBar;
    bool ChantTimerPaused;
    long PausedTime;
    long BriarState;
    int  m_terminateAfter;          // test control: frames until the thread is terminating (-1 never)
    bool m_pausedSeenDuringWait;
    bool IsActiveThreadTerminating() { return m_terminateAfter >= 0 && Game->m_frames >= m_terminateAfter; }
    void PauseChantTimer()
    {
        PausedTime = g_pThingManager_0143e8f8->GetTimerValue(ChantTimer);
        ChantTimerPaused = true;
        while (BriarState == 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ChantTimerPaused = false;
    }
};

void CGameScriptInterface_Calls::NewScriptFrame()
{
    ++m_frames;
    if (m_pScript->ChantTimerPaused) m_pScript->m_pausedSeenDuringWait = true;
    if (m_frames == 3) m_pScript->BriarState = 2;   // Briar Rose leaves state 1 after three frames
}

int main()
{
    CGameScriptInterface_Calls game; CQ_MinionCampScript s;
    g_thingManager.m_timerValue = 100;
    game.m_pScript = &s; game.m_frames = 0;
    s.Game = &game; s.ChantTimer = 5; s.ChantTimerPaused = false; s.PausedTime = 0; s.BriarState = 1;
    s.m_terminateAfter = -1; s.m_pausedSeenDuringWait = false;
    s.PauseChantTimer();
    if (s.PausedTime != 105 || game.m_frames != 3 || !s.m_pausedSeenDuringWait || s.ChantTimerPaused) { std::printf("00da2490_TEST FAIL\n"); return 1; }
    // terminated while waiting: the flag stays raised
    game.m_frames = 0; s.BriarState = 1; s.m_terminateAfter = 2; s.ChantTimerPaused = false;
    s.PauseChantTimer();
    if (game.m_frames != 2 || !s.ChantTimerPaused) { std::printf("00da2490_TEST FAIL\n"); return 1; }
    // not in state 1: no wait, flag cleared at once
    game.m_frames = 0; s.BriarState = 0; s.m_terminateAfter = -1;
    s.PauseChantTimer();
    if (game.m_frames != 0 || s.ChantTimerPaused) { std::printf("00da2490_TEST FAIL\n"); return 1; }
    std::printf("00da2490_TEST PASS\n");
    return 0;
}