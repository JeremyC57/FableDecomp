// Behaviour test for CV_GhostGrannyNecklaceScript::Main (0x00E8B9C0).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CV_GhostGrannyNecklaceScript;
class CGameScriptInterface { public:
    bool* necklace; int returnAfter; int leaveAfter;
    void NewScriptFrame() { Log("frame"); if (--returnAfter == 0) *necklace = true; if (*necklace) --leaveAfter; }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "OrchardFarm") == 0 && leaveAfter > 0; }
    void PostAddScriptedEntities() { Log("gpost"); }
    const char* GetActiveQuestName() { return "Q"; }
    void SetQuestAsCompleted(const char* q, bool a, bool b, bool c) { Log("complete"); Log(q); if (a || !b || c) Log("badargs"); }
    void DeactivateQuestLater(const char* q, unsigned long d) { Log("deactivate"); Log(q); if (d) Log("badargs"); }
};
struct CV_GhostGrannyNecklaceScript {
    CGameScriptInterface* Game;
    bool NecklaceReturned;
    int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void SpawnFunction(const char* name) { Log("spawn"); Log(name); }
    void AddEntityScriptBinding(const char* name) { Log("bind"); Log(name); }
    void PostAddScriptedEntities() { Log("post"); }
    void Main()
    {
        AddEntityScriptBinding("GhostGrannyNecklace");
        AddEntityScriptBinding("GhostGrannyDaughterInLaw");
        AddEntityScriptBinding("GhostGrannySon");
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        SpawnFunction("CreateBandit");
        SpawnFunction("GhostGrannyHints");
        while (!NecklaceReturned) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, true, false);
        while (!!Game->IsRegionLoaded("OrchardFarm")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
static const char* kHead = "bind;GhostGrannyNecklace;bind;GhostGrannyDaughterInLaw;bind;GhostGrannySon;post;gpost;spawn;CreateBandit;spawn;GhostGrannyHints;";
static bool Run(int returnAfter, int leaveAfter, int termAfter, const char* tail)
{
    g_log[0] = 0;
    CV_GhostGrannyNecklaceScript s; s.NecklaceReturned = false; s.termAfter = termAfter;
    CGameScriptInterface game; game.necklace = &s.NecklaceReturned; game.returnAfter = returnAfter; game.leaveAfter = leaveAfter;
    s.Game = &game;
    s.Main();
    char expect[1024]; std::strcpy(expect, kHead); std::strcat(expect, tail);
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    bool ok = Run(2, 2, 100, "frame;frame;complete;Q;frame;deactivate;Q;")
        && Run(3, 1, 1, "frame;frame;")
        && Run(1, 1, 100, "frame;complete;Q;deactivate;Q;");
    if (!ok) { std::printf("00e8b9c0_TEST FAIL\n"); return 1; }
    std::printf("00e8b9c0_TEST PASS\n");
    return 0;
}