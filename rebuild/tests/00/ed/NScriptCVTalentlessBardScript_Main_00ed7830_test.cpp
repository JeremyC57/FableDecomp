// Behaviour test for 0x00ED7830 NScript::CV_TalentlessBardScript::Main: bind TB_BardSpawnPoint and TB_Bard, post-add the
// scripted entities (script then interface), spawn WatchForCompletedQuests, wait until bBard_finished, then complete the
// active quest (show screen, give reward, not silent) and deactivate it; termination while waiting returns early.
#include <stdio.h>
#include <string.h>

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static int g_frame, g_finishFrame, g_endFrame;
struct CTalentlessBard;
static CTalentlessBard* g_script;
class CGame { public:
    void PostAddScriptedEntities() { Log("GamePost"); }
    void NewScriptFrame();
    const char* GetActiveQuestName() { return "V_TalentlessBard"; }
    void SetQuestAsCompleted(const char* q, bool a, bool b, bool c) { Log(q); if (!(a && b && !c)) Log("FlagsBad"); Log("Completed"); }
    void DeactivateQuestLater(const char* q, long d) { Log(q); if (d != 0) Log("DelayBad"); Log("Deactivate"); }
};
struct CTalentlessBard {
    CGame* Game;
    bool bHurtBard;
    bool bBard_finished;
    bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
    void AddEntityScriptBinding(const char* name, unsigned long flags) { Log(name); if (flags) Log("FlagsBad"); }
    void PostAddScriptedEntities() { Log("ScriptPost"); }
    void SpawnFunction(const char* name) { Log(name); }
    void Main()
    {
        AddEntityScriptBinding("TB_BardSpawnPoint", 0);
        AddEntityScriptBinding("TB_Bard", 0);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        SpawnFunction("WatchForCompletedQuests");
        while (bBard_finished != true) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
void CGame::NewScriptFrame() { ++g_frame; if (g_frame >= g_finishFrame) g_script->bBard_finished = true; }

int main()
{
    CGame g; CTalentlessBard s; s.Game = &g; s.bHurtBard = false; s.bBard_finished = false; g_script = &s;
    g_log[0] = 0; g_frame = 0; g_finishFrame = 4; g_endFrame = 100;
    s.Main();
    int ok = !strcmp(g_log, "TB_BardSpawnPoint;TB_Bard;ScriptPost;GamePost;WatchForCompletedQuests;V_TalentlessBard;Completed;V_TalentlessBard;Deactivate;")
        && g_frame == 4;
    g_log[0] = 0; g_frame = 0; g_finishFrame = 100; g_endFrame = 3; s.bBard_finished = false;
    s.Main();
    ok = ok && strstr(g_log, ";Completed;") == 0 && g_frame == 3;
    if (!ok) { printf("00ed7830_TEST FAIL\n"); return 1; }
    printf("00ed7830_TEST PASS\n");
    return 0;
}