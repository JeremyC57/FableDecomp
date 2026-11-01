// Behaviour test for 0x00D89E60 NScript::CQ_HerosOldHouseScript::Main: the quest fails once when the wife is attacked while helping
#include <cstdio>
#include <cstring>
static int gFrames = 0, gBindings = 0, gSpawns = 0, gFailed = 0, gDeactivated = 0;
struct CScript;
class CGame { public:
    CScript* Script;
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "OakBay") == 0 && gFrames >= 1; }
    void NewScriptFrame();
    void PostAddScriptedEntities() {}
    void SetQuestAsFailed(const char* q, bool, const char* reason, bool) { if (std::strcmp(q, "Q_HerosOldHouse") == 0 && reason[0] == 0) ++gFailed; }
    void DeactivateQuestLater(const char* q, unsigned long) { if (std::strcmp(q, "Q_HerosOldHouse") == 0) ++gDeactivated; }
    const char* GetActiveQuestName() { return "Q_HerosOldHouse"; }
};
struct CScript {
    CGame* Game; int TerminateAt;
    bool Helping, Helped, MissionAborted, WifeAttacked, WifeAttackedAndLeft, BootyDugUp;
    bool IsActiveThreadTerminating() { return gFrames >= TerminateAt; }
    void AddEntityScriptBinding(const char*, unsigned long flags) { if (flags == 1) ++gBindings; }
    void AddSpawnedFunction(const char* name) { if (std::strcmp(name, "WatchForBooty") == 0) ++gSpawns; }
    void PostAddScriptedEntities() {}
    void Main()
    {
        while (!Game->IsRegionLoaded("OakBay")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        AddEntityScriptBinding("GhostFisherman", 1); AddEntityScriptBinding("FishermansWife", 1); AddEntityScriptBinding("ExtraBooty", 1);
        PostAddScriptedEntities(); Game->PostAddScriptedEntities();
        AddSpawnedFunction("WatchForBooty");
        MissionAborted = false;
        bool questFailed = false;
        while (!BootyDugUp) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (!questFailed && Game->IsRegionLoaded("OakBay") && WifeAttacked) {
                if (IsActiveThreadTerminating()) return;
                if (Helping) {
                    if (IsActiveThreadTerminating()) return;
                    Game->SetQuestAsFailed(Game->GetActiveQuestName(), false, "", true);
                    questFailed = true;
                    Helping = false;
                }
            }
        }
        if (IsActiveThreadTerminating()) return;
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
void CGame::NewScriptFrame()
{
    ++gFrames;
    if (gFrames == 3) { Script->WifeAttacked = true; Script->Helping = true; }
    if (gFrames == 5) Script->Helping = true;   // already failed: no second failure
    if (gFrames == 8) Script->BootyDugUp = true;
}
int main()
{
    CGame g; CScript s; std::memset(&s, 0, sizeof(s)); s.Game = &g; g.Script = &s; s.TerminateAt = 100; s.MissionAborted = true;
    s.Main();
    if (gBindings != 3 || gSpawns != 1 || gFailed != 1 || gDeactivated != 1 || s.MissionAborted || gFrames != 8) { std::printf("00d89e60_TEST FAIL\n"); return 1; }
    std::printf("00d89e60_TEST PASS\n");
    return 0;
}