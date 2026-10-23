// Behaviour test for CV_ExposeMayorScript::PhaseFive (0x00E7A740): the three path arms through a logging stub.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing { const char* name; };
struct CMasterScriptData { bool MayorBribePathTaken, MayorExposePathTaken; };
struct CGameScriptInterface {
    int loadedAfter;
    void NewScriptFrame() { Log("frame"); }
    bool IsLevelLoaded(const char*) { return loadedAfter-- <= 0; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.name = n; return t; }
    void RemoveThing(const CScriptThing& t, bool, bool) { Log("remove"); Log(t.name); }
    void AddScreenMessage(const char* m, long) { Log(m); }
    void DeactivateQuest(const char* q, unsigned long) { Log("deactivate"); Log(q); }
    void DeactivateQuestLater(const char* q, unsigned long) { Log("later"); Log(q); }
    void RemoveQuestCardFromHero(const char* q) { Log("card"); Log(q); }
    void SetQuestCardObjective(const char* q, const char* o, const char* r1, const char* r2) { Log(q); Log(o); Log(r1); Log(r2); }
    const char* GetActiveQuestName() { return "V_ExposeMayor"; }
};
struct CV_ExposeMayorScript {
    CGameScriptInterface* Game;
    CMasterScriptData* PMasterData;
    long Path;
    bool IsActiveThreadTerminating() { return false; }
    void PhaseFive()
    {
        if (Path == 1) {
            if (IsActiveThreadTerminating()) return;
            Game->DeactivateQuest("V_ParanoidWhispers", 0);
            Game->DeactivateQuestLater("V_MayorsInvitation", 0);
            Game->DeactivateQuestLater("V_HeroDuel", 0);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "OBJECTIVE_05", "BowerstonePosh", "");
            while (!Game->IsLevelLoaded("BowerstonePosh")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            CScriptThing ladyGrey = Game->GetThingWithScriptName("LadyGrey");
            Game->RemoveThing(ladyGrey, false, true);
            PMasterData->MayorExposePathTaken = true;
        } else if (Path == 2) {
            if (IsActiveThreadTerminating()) return;
            Game->DeactivateQuest("V_ParanoidWhispers", 0);
            PMasterData->MayorBribePathTaken = true;
            Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->AddScreenMessage("no path", 0);
        }
    }
};
static bool Run(long path, const char* expect, bool expose, bool bribe)
{
    g_log[0] = 0;
    CMasterScriptData data; data.MayorBribePathTaken = data.MayorExposePathTaken = false;
    CGameScriptInterface game; game.loadedAfter = 1;
    CV_ExposeMayorScript s; s.Game = &game; s.PMasterData = &data; s.Path = path;
    s.PhaseFive();
    return std::strcmp(g_log, expect) == 0 && data.MayorExposePathTaken == expose && data.MayorBribePathTaken == bribe;
}
int main()
{
    bool ok = Run(1, "deactivate;V_ParanoidWhispers;later;V_MayorsInvitation;later;V_HeroDuel;V_ExposeMayor;OBJECTIVE_05;BowerstonePosh;;frame;remove;LadyGrey;", true, false)
        && Run(2, "deactivate;V_ParanoidWhispers;card;V_ExposeMayor;", false, true)
        && Run(0, "no path;", false, false);
    if (!ok) { std::printf("00e7a740_TEST FAIL\n"); return 1; }
    std::printf("00e7a740_TEST PASS\n");
    return 0;
}
