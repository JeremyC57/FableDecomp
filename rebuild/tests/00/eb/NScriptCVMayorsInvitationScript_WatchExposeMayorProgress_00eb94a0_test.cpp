// Behaviour test for CV_MayorsInvitationScript::WatchExposeMayorProgress (0x00EB94A0).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CMasterScriptData { bool HeroDuelDone, MayorBribePathTaken; };
class CGameScriptInterface { public:
    CMasterScriptData* data;
    void NewScriptFrame() { Log("frame"); data->MayorBribePathTaken = true; }
    void GiveHeroQuestCardDirectly(const char* card, const char* quest, bool) { Log(card); Log(quest); }
    void SetQuestCardObjective(const char* q, const char* o, const char*, const char*) { Log(o); }
    const char* GetActiveQuestName() { return "V_MayorsInvitation"; }
};
struct CV_MayorsInvitationScript {
    CGameScriptInterface* Game;
    CMasterScriptData* PMasterData;
    bool QuestCardGiven;
    bool IsActiveThreadTerminating() { return false; }
    void SpawnFunction(const char* name) { Log("spawn"); Log(name); }
    void WatchExposeMayorProgress()
    {
        while (!PMasterData->MayorBribePathTaken) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (QuestCardGiven) return;
        if (IsActiveThreadTerminating()) return;
        Game->GiveHeroQuestCardDirectly("CARD", Game->GetActiveQuestName(), false);
        if (PMasterData->HeroDuelDone) {
            if (IsActiveThreadTerminating()) return;
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "OBJECTIVE_09", "", "");
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "OBJECTIVE_07", "", "");
            SpawnFunction("MonitorThunderHeroDuel");
        }
        QuestCardGiven = true;
    }
};
static bool Run(bool given, bool duelDone, const char* expect)
{
    g_log[0] = 0;
    CMasterScriptData data; data.HeroDuelDone = duelDone; data.MayorBribePathTaken = false;
    CGameScriptInterface game; game.data = &data;
    CV_MayorsInvitationScript s; s.Game = &game; s.PMasterData = &data; s.QuestCardGiven = given;
    s.WatchExposeMayorProgress();
    return std::strcmp(g_log, expect) == 0 && s.QuestCardGiven;
}
int main()
{
    bool ok = Run(false, true, "frame;CARD;V_MayorsInvitation;OBJECTIVE_09;")
        && Run(false, false, "frame;CARD;V_MayorsInvitation;OBJECTIVE_07;spawn;MonitorThunderHeroDuel;")
        && Run(true, false, "frame;");
    if (!ok) { std::printf("00eb94a0_TEST FAIL\n"); return 1; }
    std::printf("00eb94a0_TEST PASS\n");
    return 0;
}