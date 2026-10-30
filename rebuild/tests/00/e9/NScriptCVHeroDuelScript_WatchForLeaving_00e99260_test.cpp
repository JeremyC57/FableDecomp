// Behaviour test for 0x00E99260 NScript::CV_HeroDuelScript::WatchForLeaving: wait for the duel to start, then each frame:
// a dead hero fails the mission (MissionFailed, MissionOver, master LostToThunder), a succeeded mission just ends the watch;
// either way the quest card objective is reset to the mayor's invitation (objective 09, BowerstonePosh) and the duel is
// cleaned up; termination returns without either.
#include <stdio.h>
#include <string.h>

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static int g_frame, g_startFrame, g_heroDeadFrame, g_successFrame, g_endFrame;
struct CHeroDuel;
static CHeroDuel* g_script;
struct CMaster { bool LostToThunder; };
class CThing { public: bool IsAlive() const { return g_frame < g_heroDeadFrame; } };
class CGame { public:
    void NewScriptFrame();
    CThing GetHero() { return CThing(); }
    bool IsLevelLoaded(const char*) { return true; }
    void SetQuestCardObjective(const char* q, const char* o, const char* r1, const char* r2) { Log(q); Log(o); Log(r1); Log(r2); }
};
struct CHeroDuel {
    CGame* Game;
    CMaster* MasterScriptData;
    bool MissionSucceeded, MissionFailed, MissionOver, GotToExecutionTree, DuelStarted;
    bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
    void CleanUpHeroDuel() { Log("CleanUp"); }
    void WatchForLeaving()
    {
        bool heroDied = false;
        while (!DuelStarted) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        for (;;) {
            if (!Game->GetHero().IsAlive()) { heroDied = true; break; }
            if (!Game->IsLevelLoaded("HeroDualArea") && !Game->IsLevelLoaded("HeroDualAreaCave")) { }
            if (MissionSucceeded)
                break;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (heroDied) {
            MissionFailed = true;
            MissionOver = true;
            MasterScriptData->LostToThunder = true;
        }
        Game->SetQuestCardObjective("V_MayorsInvitation", "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_09", "BowerstonePosh", "");
        CleanUpHeroDuel();
    }
};
void CGame::NewScriptFrame()
{
    ++g_frame;
    if (g_frame >= g_startFrame) g_script->DuelStarted = true;
    if (g_frame >= g_successFrame) g_script->MissionSucceeded = true;
}
static void Reset(CHeroDuel& s, CMaster& m)
{
    s.MissionSucceeded = s.MissionFailed = s.MissionOver = s.GotToExecutionTree = s.DuelStarted = false;
    m.LostToThunder = false; g_log[0] = 0; g_frame = 0;
}

int main()
{
    CGame g; CMaster m; CHeroDuel s; s.Game = &g; s.MasterScriptData = &m; g_script = &s;
    const char* tail = "V_MayorsInvitation;TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_09;BowerstonePosh;;CleanUp;";
    Reset(s, m); g_startFrame = 2; g_heroDeadFrame = 5; g_successFrame = 100; g_endFrame = 100;
    s.WatchForLeaving();
    int ok = !strcmp(g_log, tail) && s.MissionFailed && s.MissionOver && m.LostToThunder && g_frame == 5;
    Reset(s, m); g_startFrame = 1; g_heroDeadFrame = 100; g_successFrame = 4; g_endFrame = 100;
    s.WatchForLeaving();
    ok = ok && !strcmp(g_log, tail) && !s.MissionFailed && !m.LostToThunder && g_frame == 4;
    Reset(s, m); g_startFrame = 1; g_heroDeadFrame = 100; g_successFrame = 100; g_endFrame = 6;
    s.WatchForLeaving();
    ok = ok && g_log[0] == 0 && g_frame == 6;
    if (!ok) { printf("00e99260_TEST FAIL\n"); return 1; }
    printf("00e99260_TEST PASS\n");
    return 0;
}