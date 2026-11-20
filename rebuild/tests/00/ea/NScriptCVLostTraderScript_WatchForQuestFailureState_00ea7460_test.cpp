#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_frame = 0;
static int g_termAt = 1000;
static void (*g_hook)(int frame) = 0;
static void Log(const std::string& s) { char b[16]; std::sprintf(b, "@%d", g_frame); g_log += s; g_log += b; g_log += ";"; }
static void Reset(int termAt) { g_log = ""; g_frame = 0; g_termAt = termAt; g_hook = 0; }
class CCharString { public:
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
class CScriptThing { public:
    std::string Name;
    CScriptThing() { }
    CScriptThing(const std::string& name) : Name(name) { }
};
class CScriptBase { public:
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
};
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log.c_str()); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CV_LostTraderScript::WatchForQuestFailureState waits for the failure flag, fails the active quest
// with the message for the recorded reason, deactivates it and adds the Lost Trader quest card
typedef unsigned short wchar16;
static bool g_bQuestFailed = false;
class CGameScriptInterface { public:
    std::string Message;
    void NewScriptFrame() { ++g_frame; if (g_frame == 3) g_bQuestFailed = true; }
    CCharString GetActiveQuestName() { return CCharString("Q_LOSTTRADER"); }
    void SetQuestAsFailed(const CCharString& quest, bool a, const wchar16* message, bool b)
    {
        Message = "";
        for (const wchar16* p = message; *p; ++p) Message += (char)*p;
        Log("fail " + quest.S + " " + Message); (void)a; (void)b;
    }
    void DeactivateQuestLater(const CCharString& quest, long delay) { Log("deactivate " + quest.S); (void)delay; }
    void AddQuestCard(const CCharString& card, const CCharString& quest, bool a, bool b) { Log("card " + card.S + " " + quest.S); (void)a; (void)b; }
};
static const wchar16* W(const char* text)
{
    static wchar16 buffers[4][64];
    static int next = 0;
    wchar16* out = buffers[next++ & 3];
    int i = 0;
    for (; text[i]; ++i) out[i] = (wchar16)(unsigned char)text[i];
    out[i] = 0;
    return out;
}
struct CV_LostTraderScript : CScriptBase {
    CGameScriptInterface* Game;
    long ReasonForFailure;
    void WatchForQuestFailureState(CScriptThing trader);
};
void CV_LostTraderScript::WatchForQuestFailureState(CScriptThing trader)
{
    while (!g_bQuestFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (ReasonForFailure == 0) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, W("TEXT_QST_042_QUEST_FAILED_TRADER_DIED"), true);
    } else if (ReasonForFailure == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, W("TEXT_QST_042_QUEST_FAILED_TRADERS_BROTHER_DIED"), true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, W(""), true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->AddQuestCard("OBJECT_QUEST_CARD_LOST_TRADER", "V_LostTrader", false, false);
}
int main()
{
    CGameScriptInterface game;
    CV_LostTraderScript script;
    script.Game = &game;
    static const char* const reasons[3] = { "TEXT_QST_042_QUEST_FAILED_TRADER_DIED", "TEXT_QST_042_QUEST_FAILED_TRADERS_BROTHER_DIED", "" };
    for (long r = 0; r < 3; ++r) {
        Reset(1000); g_bQuestFailed = false; script.ReasonForFailure = r == 2 ? 7 : r;
        script.WatchForQuestFailureState(CScriptThing("TRADER"));
        std::string expect = std::string("fail Q_LOSTTRADER ") + reasons[r] + "@3;deactivate Q_LOSTTRADER@3;card OBJECT_QUEST_CARD_LOST_TRADER V_LostTrader@3;";
        CHECK("00ea7460", g_log == expect);
    }
    Reset(2); g_bQuestFailed = false; script.ReasonForFailure = 0;
    script.WatchForQuestFailureState(CScriptThing("TRADER"));
    CHECK("00ea7460", g_log == "" && g_frame == 2);
    std::printf("00ea7460_TEST PASS\n");
    return 0;
}