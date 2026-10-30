// Behaviour test for 0x00E268C0 NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::Main: bind MazeAtTavern, post-add
// (script then interface), set the active quest's card objective (second objective text, "" and "OakBay"), wait until the
// guardian has spoken to the hero, wait while Maze is alive, then complete the active quest (all flags false) and
// deactivate it with no delay; termination during either wait returns early.
#include <stdio.h>
#include <string.h>

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static int g_frame, g_spokeFrame, g_mazeDeadFrame, g_endFrame;
struct CSister;
static CSister* g_script;
class CThing { public: bool IsAlive() const { return g_frame < g_mazeDeadFrame; } };
class CGame { public:
    void PostAddScriptedEntities() { Log("GamePost"); }
    void NewScriptFrame();
    const char* GetActiveQuestName() { return "Q_Sister"; }
    CThing GetThingWithScriptName(const char* n) { Log(n); return CThing(); }
    void SetQuestCardObjective(const char* q, const char* o, const char* r1, const char* r2) { Log(q); Log(o); Log(r1); Log(r2); }
    void SetQuestAsCompleted(const char* q, bool a, bool b, bool c) { Log(q); if (a || b || c) Log("FlagsBad"); Log("Completed"); }
    void DeactivateQuestLater(const char* q, long d) { Log(q); if (d) Log("DelayBad"); Log("Deactivate"); }
};
struct CSister {
    CGame* Game;
    bool GuardianSpokeToHero;
    bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
    void AddEntityScriptBinding(const char* name, unsigned long flags) { Log(name); if (flags) Log("FlagsBad"); }
    void PostAddScriptedEntities() { Log("ScriptPost"); }
    void Main()
    {
        AddEntityScriptBinding("MazeAtTavern", 0);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GUARDIAN_SISTER_INFO_SECOND_OBJECTIVE_01", "", "OakBay");
        while (!GuardianSpokeToHero) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CThing maze = Game->GetThingWithScriptName("MazeAtTavern");
        while (maze.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
void CGame::NewScriptFrame() { ++g_frame; if (g_frame >= g_spokeFrame) g_script->GuardianSpokeToHero = true; }

int main()
{
    CGame g; CSister s; s.Game = &g; g_script = &s;
    s.GuardianSpokeToHero = false; g_log[0] = 0; g_frame = 0; g_spokeFrame = 3; g_mazeDeadFrame = 6; g_endFrame = 100;
    s.Main();
    int ok = !strcmp(g_log, "MazeAtTavern;ScriptPost;GamePost;Q_Sister;TEXT_QUEST_GUARDIAN_SISTER_INFO_SECOND_OBJECTIVE_01;;OakBay;"
                            "MazeAtTavern;Q_Sister;Completed;Q_Sister;Deactivate;") && g_frame == 6;
    s.GuardianSpokeToHero = false; g_log[0] = 0; g_frame = 0; g_spokeFrame = 2; g_mazeDeadFrame = 50; g_endFrame = 4;
    s.Main();
    ok = ok && strstr(g_log, "Completed") == 0 && g_frame == 4;
    if (!ok) { printf("00e268c0_TEST FAIL\n"); return 1; }
    printf("00e268c0_TEST PASS\n");
    return 0;
}