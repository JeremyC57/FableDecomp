// Behaviour test for CV_ExposeMayorScript::EndMission (0x00E7B050): the two quest paths through a logging stub.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing { const char* name; };
struct CMasterScriptData { bool MayorBribed; };
struct CGameScriptInterface {
    CMasterScriptData* data;
    int frame;
    void NewScriptFrame() { ++frame; Log("frame"); if (frame == 2) data->MayorBribed = true; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.name = n; return t; }
    void SetCategoryActivity(const char* c, bool) { Log(c); }
    void SetThingAsUsable(const CScriptThing& t, bool) { Log("usable"); Log(t.name); }
    const char* GetActiveQuestName() { return "V_ExposeMayor"; }
    void SetQuestAsCompleted(const char* q, bool, bool, bool) { Log("completed"); Log(q); }
    void ClearThingHasInformation(const CScriptThing& t) { Log("clear"); Log(t.name); }
    void DeactivateQuestLater(const char* q, unsigned long) { Log("later"); Log(q); }
    void FadeScreenIn() { Log("fadein"); }
};
struct CV_ExposeMayorScript {
    CGameScriptInterface* Game;
    CMasterScriptData* PMasterData;
    long Path;
    bool SaidYesToBecomeMayor;
    bool IsActiveThreadTerminating() { return false; }
    void PlayCutscene(const char* name, bool, const char* region) { Log(name); Log(region); }
    void EndMission()
    {
        CScriptThing door = Game->GetThingWithScriptName("GreyHouseFrontDoor");
        if (Path == 1) {
            if (IsActiveThreadTerminating()) return;
            if (SaidYesToBecomeMayor) {
                if (IsActiveThreadTerminating()) return;
                PlayCutscene("CS_MAYOREXPOSE_BECOMEMAYOR", true, "NULL");
                Game->SetCategoryActivity("ExposedMayor", true);
                Game->SetThingAsUsable(door, true);
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->SetCategoryActivity("ExposedMayorNotBecomeMayor", true);
            }
            Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
        } else {
            if (IsActiveThreadTerminating()) return;
            while (!PMasterData->MayorBribed) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Game->SetCategoryActivity("BribedMayor", true);
        }
        CScriptThing wiseVillager = Game->GetThingWithScriptName("WiseVillager");
        Game->ClearThingHasInformation(wiseVillager);
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
        if (Path == 1) {
            if (IsActiveThreadTerminating()) return;
            Game->FadeScreenIn();
        }
    }
};
static bool Run(long path, bool yes, const char* expect)
{
    g_log[0] = 0;
    CMasterScriptData data; data.MayorBribed = false;
    CGameScriptInterface game; game.data = &data; game.frame = 0;
    CV_ExposeMayorScript s; s.Game = &game; s.PMasterData = &data; s.Path = path; s.SaidYesToBecomeMayor = yes;
    s.EndMission();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    bool ok = Run(1, true, "CS_MAYOREXPOSE_BECOMEMAYOR;NULL;ExposedMayor;usable;GreyHouseFrontDoor;completed;V_ExposeMayor;clear;WiseVillager;later;V_ExposeMayor;fadein;")
        && Run(1, false, "ExposedMayorNotBecomeMayor;completed;V_ExposeMayor;clear;WiseVillager;later;V_ExposeMayor;fadein;")
        && Run(2, true, "frame;frame;BribedMayor;clear;WiseVillager;later;V_ExposeMayor;");
    if (!ok) { std::printf("00e7b050_TEST FAIL\n"); return 1; }
    std::printf("00e7b050_TEST PASS\n");
    return 0;
}
