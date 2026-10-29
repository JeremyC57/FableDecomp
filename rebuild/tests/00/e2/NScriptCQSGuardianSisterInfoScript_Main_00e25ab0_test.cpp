// Behaviour test for 0x00E25AB0 NScript::CQS_GuardianSisterInfoScript::Main: binds "MazeAtTavern" (flags 0), post-adds the
// scripted entities (script and interface), sets the quest card objective, waits for BowerstoneSlums, then sets the time of
// day to 10 unless the thread was terminated.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frame, g_loadFrame, g_endFrame;
class CGame { public:
    void NewScriptFrame() const { ++g_frame; }
    bool IsRegionLoaded(const CCharString& r) const { return std::strcmp(r.Text, "BowerstoneSlums") == 0 && g_frame >= g_loadFrame; }
    void PostAddScriptedEntities() const { Log("GamePost"); }
    CCharString GetActiveQuestName() const { return CCharString("QS_GuardianSisterInfo"); }
    void SetQuestCardObjective(const CCharString& q, const CCharString& o, const CCharString& r1, const CCharString& r2) const
    {
        Log(q.Text); Log(o.Text); Log(r1.Text); Log(r2.Text);
    }
    void SetTimeOfDay(float t) const { if (t > 9.99f && t < 10.01f) Log("Time10"); else Log("TimeBad"); }
};
struct Binding { const char* Name; unsigned long Flags; };
struct CQS_GuardianSisterInfoScript {
    CGame* Game;
    bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
    void AddEntityScriptBinding(Binding* b) { Log(b->Name); if (b->Flags != 0) Log("FlagsBad"); delete b; }
    void PostAddScriptedEntities() { Log("ScriptPost"); }
    void Main()
    {
        Binding* b = new Binding; b->Name = "MazeAtTavern"; b->Flags = 0;
        AddEntityScriptBinding(b);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01", "", "BowerstoneSlums");
        while (!Game->IsRegionLoaded("BowerstoneSlums")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetTimeOfDay(10.0f);
    }
};

int main()
{
    CGame g; CQS_GuardianSisterInfoScript s; s.Game = &g;
    g_log[0] = 0; g_frame = 0; g_loadFrame = 3; g_endFrame = 100;
    s.Main();
    int ok = !std::strcmp(g_log, "MazeAtTavern;ScriptPost;GamePost;QS_GuardianSisterInfo;TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01;;BowerstoneSlums;Time10;") && g_frame == 3;
    g_log[0] = 0; g_frame = 0; g_loadFrame = 50; g_endFrame = 2;
    s.Main();
    ok = ok && std::strstr(g_log, "Time") == 0 && g_frame == 2;
    if (!ok) { std::printf("00e25ab0_TEST FAIL\n"); return 1; }
    std::printf("00e25ab0_TEST PASS\n");
    return 0;
}