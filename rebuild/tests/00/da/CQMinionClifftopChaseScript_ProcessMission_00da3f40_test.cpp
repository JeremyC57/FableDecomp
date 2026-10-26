// Behaviour test for CQ_MinionClifftopChaseScript::ProcessMission (0x00DA3F40): the outcome handling through a logging stub.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CRGBColour { public: unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct CQ_MinionClifftopChaseScript;
class CGameScriptInterface { public:
    CQ_MinionClifftopChaseScript* script;
    int frame;
    void NewScriptFrame();
    void SetCreatureGeneratorsEnabled(const char* region, bool on) { Log(region); Log(on ? "on" : "off"); }
    void StopOverrideMusic(bool) { Log("music"); }
    bool FadeScreenOut(float, float, CRGBColour c) { Log(c.A == 255 && c.R == 0 ? "fadeout-black" : "fadeout-?"); return true; }
    void FadeScreenIn() { Log("fadein"); }
    void Pause(float) { Log("pause"); }
    const char* GetActiveQuestName() { return "Q_MinionClifftopChase"; }
    void SetQuestAsCompleted(const char* q, bool, bool, bool) { Log("completed"); Log(q); }
    void SetQuestAsFailed(const char* q, bool, const char* reason, bool) { Log("failed"); Log(q); Log(reason); }
    void DeactivateQuest(const char* q, unsigned long) { Log("deactivate"); Log(q); }
    void DeactivateQuestLater(const char* q, unsigned long) { Log("later"); Log(q); }
};
struct CQ_MinionClifftopChaseScript {
    CGameScriptInterface* Game;
    bool MapKeeperSaidDialog, MissionFailed, MissionSucceeded, IsIntroCSFinished;
    long ReasonForFailure;
    int outcomeFrame, outcome;               // test driver: which flag the game raises, and when
    bool IsActiveThreadTerminating() { return false; }
    void ProcessMission()
    {
        while (!IsIntroCSFinished) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->SetCreatureGeneratorsEnabled("GibbetWoods", false);
        Game->SetCreatureGeneratorsEnabled("DesertedFarm", false);
        while (!(MissionSucceeded || MissionFailed || MapKeeperSaidDialog)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->StopOverrideMusic(false);
        if (MapKeeperSaidDialog) {
            if (IsActiveThreadTerminating()) return;
            while (!MissionSucceeded) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
        }
        Game->SetCreatureGeneratorsEnabled("GibbetWoods", true);
        Game->SetCreatureGeneratorsEnabled("DesertedFarm", true);
        Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
        if (MissionSucceeded) {
            if (IsActiveThreadTerminating()) return;
            Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->DeactivateQuest("Q_MinionClifftopChase_Barriers", 0);
            if (ReasonForFailure == 0) { if (IsActiveThreadTerminating()) return; Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, "ARCH_DIED", true); }
            else if (ReasonForFailure == 1) { if (IsActiveThreadTerminating()) return; Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, "TIME_RAN_OUT", true); }
            else { if (IsActiveThreadTerminating()) return; Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, "", true); }
        }
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
        Game->Pause(0.3f);
        Game->FadeScreenIn();
    }
};
void CGameScriptInterface::NewScriptFrame()
{
    ++frame;
    if (frame == 1) script->IsIntroCSFinished = true;
    if (frame == 3) {
        if (script->outcome == 0) script->MissionSucceeded = true;
        if (script->outcome == 1) script->MissionFailed = true;
        if (script->outcome == 2) script->MapKeeperSaidDialog = true;
    }
    if (frame == 5 && script->outcome == 2) script->MissionSucceeded = true;
}
static bool Run(int outcome, long reason, const char* expect)
{
    g_log[0] = 0;
    CGameScriptInterface game;
    CQ_MinionClifftopChaseScript s;
    game.script = &s; game.frame = 0;
    s.Game = &game;
    s.MapKeeperSaidDialog = s.MissionFailed = s.MissionSucceeded = s.IsIntroCSFinished = false;
    s.ReasonForFailure = reason;
    s.outcome = outcome;
    s.ProcessMission();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    const char* head = "GibbetWoods;off;DesertedFarm;off;music;GibbetWoods;on;DesertedFarm;on;fadeout-black;";
    char e1[512], e2[512], e3[512], e4[512];
    std::strcpy(e1, head); std::strcat(e1, "completed;Q_MinionClifftopChase;later;Q_MinionClifftopChase;pause;fadein;");
    std::strcpy(e2, head); std::strcat(e2, "deactivate;Q_MinionClifftopChase_Barriers;failed;Q_MinionClifftopChase;TIME_RAN_OUT;later;Q_MinionClifftopChase;pause;fadein;");
    std::strcpy(e3, head); std::strcat(e3, "deactivate;Q_MinionClifftopChase_Barriers;failed;Q_MinionClifftopChase;ARCH_DIED;later;Q_MinionClifftopChase;pause;fadein;");
    std::strcpy(e4, head); std::strcat(e4, "deactivate;Q_MinionClifftopChase_Barriers;failed;Q_MinionClifftopChase;;later;Q_MinionClifftopChase;pause;fadein;");
    bool ok = Run(0, 0, e1) && Run(2, 0, e1) && Run(1, 1, e2) && Run(1, 0, e3) && Run(1, 7, e4);
    if (!ok) { std::printf("00da3f40_TEST FAIL\n"); return 1; }
    std::printf("00da3f40_TEST PASS\n");
    return 0;
}