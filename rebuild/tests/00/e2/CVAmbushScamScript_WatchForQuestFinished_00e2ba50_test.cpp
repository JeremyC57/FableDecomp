// Behaviour test for the script watch at 0x00E2BA50.
#include <cstdio>

struct CScript;
static int g_frames = 0;
static int g_terminateAtFrame = -1;     // IsActiveThreadTerminating() true once g_frames reaches this (-1: never)
static int g_terminateCalls = 0;
static CScript* g_script = 0;
static void OnFrame();

struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; OnFrame(); }
};

struct CScript {
    CGameScriptInterface* Game;
    bool QuestFinished, TricksterDeadOrLeft, TricksterComplete, TricksterKilledBeforeAmbush, TricksterKilledAfterAmbush, AllAmbushersDead;
    CScript() : QuestFinished(false), TricksterDeadOrLeft(false), TricksterComplete(false), TricksterKilledBeforeAmbush(false), TricksterKilledAfterAmbush(false), AllAmbushersDead(false) { }
    bool IsActiveThreadTerminating() { ++g_terminateCalls; return g_terminateAtFrame >= 0 && g_frames >= g_terminateAtFrame; }
    void Watch();
};

static int g_setAt = 0;
static void OnFrame() { if (g_frames == g_setAt) { g_script->AllAmbushersDead = true; } if (g_frames == g_setAt + 2) g_script->TricksterDeadOrLeft = true; }

void CScript::Watch()
{
    while (!IsActiveThreadTerminating()) {
        if ((AllAmbushersDead && TricksterDeadOrLeft) || TricksterKilledBeforeAmbush || TricksterKilledAfterAmbush
            || TricksterComplete) {
            if (IsActiveThreadTerminating())
                return;
            QuestFinished = true;
            if (IsActiveThreadTerminating())
                return;
            return;
        }
        Game->NewScriptFrame();
    }
}

static void Reset(CScript& s, int setAt, int terminateAt)
{
    CScript z; z.Game = s.Game; s = z;
    g_frames = 0; g_terminateCalls = 0; g_setAt = setAt; g_terminateAtFrame = terminateAt; g_script = &s;
}

int main()
{
    CGameScriptInterface gsi;
    CScript s; s.Game = &gsi;
    Reset(s, 1, -1); s.Watch();
    if (g_frames != 3 || !s.QuestFinished) { std::printf("00e2ba50_TEST FAIL\n"); return 1; }
    Reset(s, 99, 5); s.Watch();
    if (g_frames != 5 || s.QuestFinished) { std::printf("00e2ba50_TEST FAIL\n"); return 1; }
    Reset(s, 99, -1); s.TricksterComplete = true; s.Watch();
    if (g_frames != 0 || !s.QuestFinished || g_terminateCalls != 3) { std::printf("00e2ba50_TEST FAIL\n"); return 1; }

    std::printf("00e2ba50_TEST PASS\n");
    return 0;
}