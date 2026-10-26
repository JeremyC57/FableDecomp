// Behaviour test for CQ_OrchardFarmRaidScript::ReplaceQuestCards (0x00DD0EB0).
#include <cstdio>
#include <cstring>
// Test doubles: strings keep their literal, a script thing is a named handle whose liveness is shared with the
// test, the game script interface records calls in g_log, and the thread terminates after a set number of tests.
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void ResetLog() { g_log[0] = 0; }
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long = -1) : Text(text) {}
};
class CWideString { public:
    const void* Text;
    CWideString(const void* text) : Text(text) {}
};
class CScriptThing { public:
    const char* Name;
    const bool* AliveFlag;
    CScriptThing() : Name(""), AliveFlag(0) {}
    CScriptThing(const char* n, const bool* a) : Name(n), AliveFlag(a) {}
    bool IsAlive() const { return AliveFlag && *AliveFlag; }
};
class CScriptBase { public:
    long TerminateAfter;                                 // IsActiveThreadTerminating() turns true after this many calls
    long TerminateCalls;
    CScriptBase() : TerminateAfter(1000), TerminateCalls(0) {}
    bool IsActiveThreadTerminating() { return ++TerminateCalls > TerminateAfter; }
};
// Frame-driven world: NewScriptFrame advances the frame counter and fires one scheduled flag change.
struct CFrameEvents {
    long Frames;
    bool* Flag[4]; long At[4]; bool Value[4];
    CFrameEvents() : Frames(0) { for (int i = 0; i < 4; ++i) { Flag[i] = 0; At[i] = 0; Value[i] = false; } }
    void Schedule(int i, bool* f, long at, bool v) { Flag[i] = f; At[i] = at; Value[i] = v; }
    void Tick() { ++Frames; for (int i = 0; i < 4; ++i) if (Flag[i] && At[i] == Frames) *Flag[i] = Value[i]; }
};
class CGameScriptInterface { public:
    void AddQuestCard(const CCharString& card, const CCharString& quest, bool a, bool b)
    { char t[128]; std::sprintf(t, "%s/%s/%d/%d", card.Text, quest.Text, a, b); Log(t); }
};
struct CQuestScriptBase : CScriptBase { CGameScriptInterface* GSI; };
struct CQ_OrchardFarmRaidScript : CQuestScriptBase { void ReplaceQuestCards(); };
void CQ_OrchardFarmRaidScript::ReplaceQuestCards()
{
    GSI->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false);
    GSI->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false);
}
int main()
{
    CGameScriptInterface g; CQ_OrchardFarmRaidScript q; q.GSI = &g; ResetLog();
    q.ReplaceQuestCards();
    if (std::strcmp(g_log, "OBJECT_QUEST_CARD_PROTECT_FARM/Q_OrchardFarmRaidGood/0/0;OBJECT_QUEST_CARD_PROTECT_FARM_EVIL/Q_OrchardFarmRaidEvil/0/0;")) {
        std::printf("00dd0eb0_TEST FAIL\n"); return 1;
    }
    std::printf("00dd0eb0_TEST PASS\n");
    return 0;
}