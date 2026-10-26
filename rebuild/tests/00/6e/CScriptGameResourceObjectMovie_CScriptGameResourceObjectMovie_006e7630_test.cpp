#include <cstdio>
// Behaviour model of 0x006E7630: CScriptGameResourceObjectMovie(scriptInterface, setConversationInputProcess).
static char g_log[8]; static int g_n = 0; static long g_mode = 0; static bool g_flag = true; static long g_asked = -1;
class CPlayer { public:
    void InitialiseIfUninitialised() { g_log[g_n++] = 'I'; }
    void RemoveMode(long mode) { g_log[g_n++] = 'R'; g_mode = mode; }
    void AddMode(long mode, bool flag) { g_log[g_n++] = 'A'; g_mode = mode; g_flag = flag; }
};
class CPlayerManager { public: CPlayer players[2]; CPlayer* GetPlayer(long i) { g_asked = i; return &players[i]; } };
class CGameScriptInterface { public: CPlayerManager* PlayerManager; long CurrentPlayer; };
class CScriptGameResourceObjectMovieBase { public: virtual ~CScriptGameResourceObjectMovieBase() {} void* PImpObject; void* PImpCount; CScriptGameResourceObjectMovieBase() : PImpObject(0), PImpCount(0) {} };
struct CScriptGameResourceObjectMovie : CScriptGameResourceObjectMovieBase {
    CGameScriptInterface* ScriptInterface; bool SetConversationInputProcess;
    CScriptGameResourceObjectMovie(CGameScriptInterface* si, bool set) : ScriptInterface(si), SetConversationInputProcess(set)
    {
        if (set) { CPlayer* p = si->PlayerManager->GetPlayer(si->CurrentPlayer); p->InitialiseIfUninitialised(); p->RemoveMode(2); p->AddMode(0x1b, false); }
    }
};
int main()
{
    CPlayerManager pm; CGameScriptInterface si; si.PlayerManager = &pm; si.CurrentPlayer = 1;
    CScriptGameResourceObjectMovie a(&si, false);
    if (a.ScriptInterface != &si || a.SetConversationInputProcess || g_n != 0 || a.PImpObject != 0 || a.PImpCount != 0) { std::printf("006e7630_TEST FAIL\n"); return 1; }
    CScriptGameResourceObjectMovie b(&si, true);
    if (!b.SetConversationInputProcess || g_n != 3 || g_log[0] != 'I' || g_log[1] != 'R' || g_log[2] != 'A' || g_mode != 0x1b || g_flag || g_asked != 1)
        { std::printf("006e7630_TEST FAIL\n"); return 1; }
    std::printf("006e7630_TEST PASS\n");
    return 0;
}