// Behaviour test for NScript::CV_FishermanScript::Main 0x00E7B4E0
#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static void Log(const char* s) { g_log += s; g_log += ';'; }
class CGame { public:
    int Frames;
    int RegionLoadedFrames;                             // the region reads as loaded for this many queries
    bool* SavedFlag;                                    // set by the game after a few frames
    int SaveAfter;
    void PostAddScriptedEntities() { Log("GamePost"); }
    void NewScriptFrame() { ++Frames; if (Frames == SaveAfter) *SavedFlag = true; }
    bool IsRegionLoaded(const char* name) { Log(name); return RegionLoadedFrames-- > 0; }
    void ActivateQuest(const char* name) { Log("Activate"); Log(name); }
    const char* GetActiveQuestName() { return "V_Fisherman"; }
    void DeactivateQuestLater(const char* name, int delay) { Log("Deactivate"); Log(name); }
};
struct CV_FishermanScript {
    CGame* Game;
    bool HeroHasSavedFishermanFromWasps;
    int TerminateAtFrame;
    bool IsActiveThreadTerminating() { return TerminateAtFrame >= 0 && Game->Frames >= TerminateAtFrame; }
    void AddEntityScriptBinding(const char* name) { Log(name); }
    void PostAddScriptedEntities() { Log("Post"); }
    void Main()
    {
        AddEntityScriptBinding("Fisherman");
        AddEntityScriptBinding("M_FishermanWaspSpawn");
        AddEntityScriptBinding("FishermanBulletinBoard");
        AddEntityScriptBinding("FishermanWasp");
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        while (!HeroHasSavedFishermanFromWasps) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (Game->IsRegionLoaded("Fisherman")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->ActivateQuest("V_FishingCompetition");
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
int main()
{
    CV_FishermanScript s; CGame g;
    s.Game = &g; s.HeroHasSavedFishermanFromWasps = false; s.TerminateAtFrame = -1;
    g.Frames = 0; g.RegionLoadedFrames = 2; g.SavedFlag = &s.HeroHasSavedFishermanFromWasps; g.SaveAfter = 3;
    s.Main();
    const char* want = "Fisherman;M_FishermanWaspSpawn;FishermanBulletinBoard;FishermanWasp;Post;GamePost;"
                       "Fisherman;Fisherman;Fisherman;Activate;V_FishingCompetition;Deactivate;V_Fisherman;";
    if (g_log != want || g.Frames != 5) { std::printf("00e7b4e0_TEST FAIL\n"); return 1; }
    // terminating while waiting for the rescue: no quest change
    g_log = "";
    s.HeroHasSavedFishermanFromWasps = false; s.TerminateAtFrame = 1; g.Frames = 0; g.SaveAfter = 100;
    s.Main();
    if (g_log.find("Activate") != std::string::npos) { std::printf("00e7b4e0_TEST FAIL\n"); return 1; }
    std::printf("00e7b4e0_TEST PASS\n");
    return 0;
}