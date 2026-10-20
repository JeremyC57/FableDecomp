// Behaviour test for CQR_EscortTraderScript::EndMission (0x00E1E670): the route is closed and the quest card removed;
// success path gives the def's renown, sets MissionSucceeded and completes the quest; the failed path fails it with
// an empty message; a terminating thread stops either path after the card removal.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CWideString {
public:
    const unsigned short* PStringData;
    CWideString(const unsigned short* t) : PStringData(t) { ++g_live; }
    ~CWideString() { --g_live; }
};
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) { ++g_live; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
};
static const unsigned short g_empty[1] = { 0 };
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGameScriptInterface {
    long renown; bool emptyMsg;
    virtual void GiveHeroRenownPoints(long p) { Log("renown"); renown = p; }
    virtual void SetQuestAsCompleted(const CCharString& q, bool a, bool b, bool c) { Log("done"); Log(q.PStringData); if (a || b || c) Log("flag"); }
    virtual void SetQuestAsFailed(const CCharString& q, bool a, const CWideString& m, bool b) { Log("fail"); Log(q.PStringData); emptyMsg = m.PStringData[0] == 0; if (a || b) Log("flag"); }
    virtual void RemoveQuestCardFromHero(const CCharString& q) { Log("card"); Log(q.PStringData); }
    virtual void MiniMapAllowRouteBetweenRegions(const CCharString& f, const CCharString& t, bool a) { Log("route"); Log(f.PStringData); Log(t.PStringData); if (a) Log("allow"); }
    virtual CCharString GetActiveQuestName() { return CCharString("Q_Escort"); }
};
struct CScriptDef { long EscortTraderRenown; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
struct CScriptBase {
    bool terminating;
    bool IsActiveThreadTerminating() { Log("term?"); return terminating; }
};
struct CQR_EscortTraderScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    bool MissionSucceeded;
    bool MissionFailed;
    void EndMission();
};
void CQR_EscortTraderScript::EndMission()
{
    m_pInterface->MiniMapAllowRouteBetweenRegions("GreatwoodEntrance", "OrchardFarm", true);
    m_pInterface->RemoveQuestCardFromHero(m_pInterface->GetActiveQuestName());
    if (!MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        m_pInterface->GiveHeroRenownPoints(g_pScriptDef->EscortTraderRenown);
        MissionSucceeded = true;
        m_pInterface->SetQuestAsCompleted(m_pInterface->GetActiveQuestName(), false, false, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        m_pInterface->SetQuestAsFailed(m_pInterface->GetActiveQuestName(), false, g_empty, false);
    }
}
static int Run(bool failed, bool term, const char* want, bool succeeded)
{
    CGameScriptInterface gi; gi.renown = 0; gi.emptyMsg = false;
    CQR_EscortTraderScript s; s.m_pInterface = &gi; s.terminating = term; s.MissionFailed = failed; s.MissionSucceeded = false;
    g_log[0] = 0; g_def.EscortTraderRenown = 250;
    s.EndMission();
    if (std::strcmp(g_log, want) != 0 || s.MissionSucceeded != succeeded || g_live != 0) return 1;
    if (!failed && !term && gi.renown != 250) return 1;
    if (failed && !term && !gi.emptyMsg) return 1;
    return 0;
}
int main()
{
    const char* pre = "route;GreatwoodEntrance;OrchardFarm;allow;card;Q_Escort;term?;";
    char w1[256], w2[256];
    std::strcpy(w1, pre); std::strcat(w1, "renown;done;Q_Escort;");
    std::strcpy(w2, pre); std::strcat(w2, "fail;Q_Escort;");
    int bad = Run(false, false, w1, true) | Run(true, false, w2, false) | Run(false, true, pre, false) | Run(true, true, pre, false);
    if (bad) { std::printf("00e1e670_TEST FAIL\n"); return 1; }
    std::printf("00e1e670_TEST PASS\n");
    return 0;
}
