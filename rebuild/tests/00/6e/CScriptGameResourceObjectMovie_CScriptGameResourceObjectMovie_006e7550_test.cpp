// Behaviour test for CScriptGameResourceObjectMovie::~CScriptGameResourceObjectMovie (0x006E7550): end the movie sequence, and
// only when SetConversationInputProcess is set remove player mode 0x1B from the current player; then release PImp.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CPlayerModel { long RemovedMode; int Calls; void RemoveMode(long mode) { RemovedMode = mode; ++Calls; Log("M"); } };
struct CPlayerManagerModel {
    CPlayerModel Players[4]; long Asked;
    CPlayerModel& GetPlayer(long player) { Asked = player; Log("G"); return Players[player]; }
};
struct CGameScriptInterfaceModel {
    CPlayerManagerModel* PlayerManager; long CurrentPlayer; int Ended;
    long GetCurrentPlayer() const { return CurrentPlayer; }
    void InternalEndMovieSequence() { ++Ended; Log("E"); }
};
struct CInfo { unsigned long RefCount; int Deleted; };
struct CCountedModel {
    void* Data; CInfo* Info;
    ~CCountedModel() { if (Info) { if (--Info->RefCount == 0) { Info->Deleted++; Log("P"); } } Data = 0; Info = 0; }
};
struct CScriptGameResourceObjectBaseModel { virtual ~CScriptGameResourceObjectBaseModel() { Log("B"); } };
struct CMovieBaseModel : CScriptGameResourceObjectBaseModel { CCountedModel PImp; };
struct CMovieModel : CMovieBaseModel {
    CGameScriptInterfaceModel* ScriptInterface; bool SetConversationInputProcess;
    virtual ~CMovieModel()
    {
        ScriptInterface->InternalEndMovieSequence();
        if (SetConversationInputProcess) {
            CPlayerModel& player = ScriptInterface->PlayerManager->GetPlayer(ScriptInterface->GetCurrentPlayer());
            player.RemoveMode(0x1b);
        }
    }
};
int main()
{
    CPlayerManagerModel pm;
    std::memset(&pm, 0, sizeof(pm));
    CGameScriptInterfaceModel si = { &pm, 2, 0 };
    CInfo info = { 1, 0 };
    {
        CMovieModel m; m.ScriptInterface = &si; m.SetConversationInputProcess = true; m.PImp.Data = &info; m.PImp.Info = &info;
    }
    if (std::strcmp(g_log, "EGMPB") != 0 || pm.Asked != 2 || pm.Players[2].RemovedMode != 0x1b || info.Deleted != 1) {
        std::printf("006e7550_TEST FAIL\n"); return 1;
    }
    g_log[0] = 0;
    {
        CMovieModel m; m.ScriptInterface = &si; m.SetConversationInputProcess = false; m.PImp.Data = 0; m.PImp.Info = 0;
    }
    if (std::strcmp(g_log, "EB") != 0 || si.Ended != 2 || pm.Players[2].Calls != 1) { std::printf("006e7550_TEST FAIL\n"); return 1; }
    std::printf("006e7550_TEST PASS\n");
    return 0;
}