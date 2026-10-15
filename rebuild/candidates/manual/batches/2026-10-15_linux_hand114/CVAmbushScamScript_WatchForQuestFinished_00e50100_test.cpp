// Behaviour test for the script watch at 0x00E50100.
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
    bool m_endConditionA, m_endConditionB, m_endConditionC;
    CScript() : QuestFinished(false), m_endConditionA(false), m_endConditionB(false), m_endConditionC(false) { }
    bool IsActiveThreadTerminating() { ++g_terminateCalls; return g_terminateAtFrame >= 0 && g_frames >= g_terminateAtFrame; }
    void Watch();
};

static int g_setAt = 0;
static void OnFrame() { if (g_frames == g_setAt) g_script->m_endConditionC = true; }

void CScript::Watch()
{
    while (!(m_endConditionA || m_endConditionB || m_endConditionC)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    QuestFinished = true;
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
    Reset(s, 4, -1); s.Watch();
    if (g_frames != 4 || !s.QuestFinished) { std::printf("00e50100_TEST FAIL\n"); return 1; }
    Reset(s, 99, 1); s.Watch();
    if (g_frames != 1 || s.QuestFinished) { std::printf("00e50100_TEST FAIL\n"); return 1; }
    Reset(s, 4, 4); s.Watch();
    if (g_frames != 4 || s.QuestFinished) { std::printf("00e50100_TEST FAIL\n"); return 1; }

    std::printf("00e50100_TEST PASS\n");
    return 0;
}
