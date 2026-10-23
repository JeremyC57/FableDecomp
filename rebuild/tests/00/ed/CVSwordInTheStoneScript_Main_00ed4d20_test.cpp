// Behaviour test for CV_SwordInTheStoneScript::Main (0x00ED4D20).
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CV_SwordInTheStoneScript;
struct CGameScriptInterface {
    const char** regions; int ri;       // region-loaded messages delivered one per frame (0 = none this frame)
    bool* taken; int takeAfter; int leaveAfter;
    bool MsgOnRegionLoaded(const char*& out) { if (regions[ri]) { out = regions[ri]; return true; } return false; }
    void NewScriptFrame() { Log("frame"); if (ri < 3) ++ri; if (--takeAfter == 0) *taken = true; if (*taken) --leaveAfter; }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "Witchwood3") == 0 && leaveAfter > 0; }
    void PostAddScriptedEntities() { Log("gpost"); }
    void HeroReceiveMessageFromGuildMaster(const char* t, const char* s, bool a, bool b) { Log(t); if (*s || !a || !b) Log("badargs"); }
    const char* GetActiveQuestName() { return "Q"; }
    void SetQuestAsCompleted(const char* q, bool a, bool b, bool c) { Log("complete"); if (a || b || c) Log("badargs"); }
    void DeactivateQuestLater(const char* q, unsigned long d) { Log("deactivate"); }
};
struct CV_SwordInTheStoneScript {
    CGameScriptInterface* Game;
    unsigned char master[0x100];
    bool HeroHasTakenSword, SwordInTheStoneIntroComplete;
    int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void SpawnFunction(const char* name) { Log("spawn"); Log(name); }
    void AddEntityScriptBinding(const char* name) { Log("bind"); Log(name); }
    void PostAddScriptedEntities() { Log("post"); }
    void Main()
    {
        AddEntityScriptBinding("SwordOfStrength");
        AddEntityScriptBinding("SwordCrowd");
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        const char* region = "";
        if (IsActiveThreadTerminating()) return;
        for (;;) {
            while (!Game->MsgOnRegionLoaded(region)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            if (std::strstr(region, "Witchwood")) break;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (IsActiveThreadTerminating()) return;
        if (!SwordInTheStoneIntroComplete) {
            if (IsActiveThreadTerminating()) return;
            Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_062_GM_INTRO", "", true, true);
            SwordInTheStoneIntroComplete = true;
        }
        SpawnFunction("WatchForSwordPickup");
        while (!(HeroHasTakenSword == true)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        master[0xc7] = true;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
        while (!!Game->IsRegionLoaded("Witchwood3")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
static const char* kHead = "bind;SwordOfStrength;bind;SwordCrowd;post;gpost;";
static bool Run(bool intro, int termAfter, const char* tail, bool expectDone)
{
    static const char* regs[] = { 0, "OakBay", "Witchwood2", 0 };
    g_log[0] = 0;
    CV_SwordInTheStoneScript s; std::memset(s.master, 0, sizeof(s.master));
    s.HeroHasTakenSword = false; s.SwordInTheStoneIntroComplete = intro; s.termAfter = termAfter;
    CGameScriptInterface game; game.regions = regs; game.ri = 0; game.taken = &s.HeroHasTakenSword; game.takeAfter = 4; game.leaveAfter = 2;
    s.Game = &game;
    s.Main();
    char expect[2048]; std::strcpy(expect, kHead); std::strcat(expect, tail);
    return std::strcmp(g_log, expect) == 0 && (s.master[0xc7] != 0) == expectDone && s.SwordInTheStoneIntroComplete;
}
int main()
{
    // frame 1: no message; frame 2: OakBay (not Witchwood) -> frame; then Witchwood2 matches.
    bool ok = Run(false, 100, "frame;frame;TEXT_QST_062_GM_INTRO;spawn;WatchForSwordPickup;frame;frame;complete;frame;deactivate;", true)
        && Run(true, 100, "frame;frame;spawn;WatchForSwordPickup;frame;frame;complete;frame;deactivate;", true)
        && Run(true, 2, "frame;", false);
    if (!ok) { std::printf("00ed4d20_TEST FAIL\n"); return 1; }
    std::printf("00ed4d20_TEST PASS\n");
    return 0;
}