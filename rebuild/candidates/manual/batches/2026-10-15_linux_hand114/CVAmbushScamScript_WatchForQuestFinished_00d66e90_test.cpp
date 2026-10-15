// Behaviour test for the script watch at 0x00D66E90.
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
    bool QuestFinished;
    bool QuestComplete;
    CScript() : QuestFinished(false), QuestComplete(false) { }
    bool IsActiveThreadTerminating() { ++g_terminateCalls; return g_terminateAtFrame >= 0 && g_frames >= g_terminateAtFrame; }
    void Watch();
};

static int g_setAt = 0;
static void OnFrame() { if (g_frames == g_setAt) g_script->QuestFinished = true; }

void CScript::Watch()
{
    while (!(QuestComplete || QuestFinished)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!QuestFinished)
        QuestComplete = true;
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
    Reset(s, 3, -1); s.Watch();
    if (g_frames != 3 || s.QuestComplete || !s.QuestFinished) { std::printf("00d66e90_TEST FAIL\n"); return 1; }
    Reset(s, 99, 2); s.Watch();
    if (g_frames != 2 || s.QuestComplete) { std::printf("00d66e90_TEST FAIL\n"); return 1; }
    Reset(s, 99, -1); s.QuestComplete = true; s.Watch();
    if (g_frames != 0 || !s.QuestComplete) { std::printf("00d66e90_TEST FAIL\n"); return 1; }

    std::printf("00d66e90_TEST PASS\n");
    return 0;
}
