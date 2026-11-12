// Behaviour test for CV_ArcheryCompetitionScript::DoCountDown (0x00E36750): a model of the game interface records the calls.
#include <cstdio>
#include <cstring>
struct TRetVoid { void Return() {} };
static int g_timer = 0, g_registered = 0, g_deregistered = 0, g_titles = 0, g_lastTitle = 0, g_go = 0, g_released = 0;
static int g_startFails = 0, g_terminateAt = -1, g_frames = 0, g_handleDestroyed = 0;
static float g_pauseTotal = 0;
struct Game_ {
    bool StartScriptingEntity() { if (g_startFails > 0) { --g_startFails; return false; } return true; }
    void NewScriptFrame() { ++g_frames; }
    void AddScreenTitleMessage(int n, float) { ++g_titles; g_lastTitle = n; }
    void AddScreenTitleMessage(const char* s, float) { if (!std::strcmp(s, "TEXT_QST_004_COUNTDOWN_40")) ++g_go; }
    void Pause(float t) { g_pauseTotal += t; if (t == 1.0f) --g_timer; }
} game;
class CTimer { public: CTimer() { ++g_registered; } ~CTimer() { ++g_deregistered; } void operator=(long v) { g_timer = v; } long GetTime() const { return g_timer; } };
struct Handle { ~Handle() { ++g_handleDestroyed; } bool IsValid() const { return true; } void Release() { ++g_released; } };
static bool T() { return g_terminateAt >= 0 && g_frames >= g_terminateAt; }
static void DoCountDown(long count)
{
    TRetVoid _Fail_Val;
    CTimer timer;
    timer = count;
    Handle seh;
    while (!game.StartScriptingEntity()) { game.NewScriptFrame(); if (T()) return _Fail_Val.Return(); }
    if (T()) return _Fail_Val.Return();
    while (timer.GetTime() > 0) {
        game.NewScriptFrame();
        if (T()) return _Fail_Val.Return();
        game.AddScreenTitleMessage(timer.GetTime() + 1, 0.6f);
        game.Pause(1.0f);
    }
    if (T()) return _Fail_Val.Return();
    game.AddScreenTitleMessage("TEXT_QST_004_COUNTDOWN_40", 1.0f);
    game.Pause(0.3f);
    if (seh.IsValid()) seh.Release();
}
static void reset() { g_timer = g_registered = g_deregistered = g_titles = g_lastTitle = g_go = g_released = 0; g_startFails = 0; g_terminateAt = -1; g_frames = 0; g_handleDestroyed = 0; g_pauseTotal = 0; }
int main()
{
    reset(); g_startFails = 2; DoCountDown(3);
    if (g_titles != 3 || g_lastTitle != 2 || g_go != 1 || g_released != 1 || g_registered != 1 || g_deregistered != 1 || g_handleDestroyed != 1 || g_frames != 5) { std::printf("00e36750_TEST FAIL\n"); return 1; }
    reset(); g_startFails = 5; g_terminateAt = 1; DoCountDown(3);
    if (g_titles != 0 || g_go != 0 || g_released != 0 || g_deregistered != 1 || g_handleDestroyed != 1) { std::printf("00e36750_TEST FAIL\n"); return 1; }
    reset(); g_terminateAt = 2; DoCountDown(5);
    if (g_titles != 1 || g_go != 0 || g_released != 0 || g_deregistered != 1) { std::printf("00e36750_TEST FAIL\n"); return 1; }
    std::printf("00e36750_TEST PASS\n");
    return 0;
}