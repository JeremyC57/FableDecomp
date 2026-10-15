// Behaviour test for the script watch at 0x00EA3490.
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
    bool QuestFinished, QuestComplete, QuestActivated;
    CScript() : QuestFinished(false), QuestComplete(false), QuestActivated(false) { }
    bool IsActiveThreadTerminating() { ++g_terminateCalls; return g_terminateAtFrame >= 0 && g_frames >= g_terminateAtFrame; }
    void Watch();
};

static int g_setAt = 0;
static void OnFrame() { if (g_frames == g_setAt) g_script->QuestComplete = true; }

void CScript::Watch()
{
    while (!(QuestFinished || QuestComplete)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    QuestActivated = true;
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
    Reset(s, 2, -1); s.Watch();
    if (g_frames != 2 || !s.QuestActivated) { std::printf("00ea3490_TEST FAIL\n"); return 1; }
    Reset(s, 99, 3); s.Watch();
    if (g_frames != 3 || s.QuestActivated) { std::printf("00ea3490_TEST FAIL\n"); return 1; }

    std::printf("00ea3490_TEST PASS\n");
    return 0;
}
