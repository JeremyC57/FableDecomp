// Behaviour test for CQ_OrchardFarmRaidScript::MakeTeamMemberComment (0x00DCDA80): a running comment timer only polls
// the thread state and returns false; otherwise a conversation is started for the speaker, the hero is added, the line
// "<code><speaker data>_<line>" is queued and the timer restarted with 5; all temporaries are released.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString {
public:
    char Text[64];
    CCharString(const char* t) { std::strcpy(Text, t); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
};
static CCharString operator+(const CCharString& l, const CCharString& r) { char b[64]; std::strcpy(b, l.Text); std::strcat(b, r.Text); return CCharString(b); }
static CCharString operator+(const CCharString& l, const char* r) { char b[64]; std::strcpy(b, l.Text); std::strcat(b, r); return CCharString(b); }
struct CBaseClass { virtual ~CBaseClass() {} };
struct CScriptThing : CBaseClass {
    const char* data;
    virtual CCharString GetDataString() const { return CCharString(data); }
};
static CScriptThing g_hero;
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGameScriptInterface {
    long timerValue, setTimer, setSecs; char text[64]; const CScriptThing* speaker;
    virtual CScriptThing& GetHero() { return g_hero; }
    virtual void SetTimer(long t, long s) { Log("set"); setTimer = t; setSecs = s; }
    virtual long GetTimer(long) { return timerValue; }
    virtual long AddNewConversation(const CScriptThing& s, bool b1, bool b2) { Log("new"); speaker = &s; if (b1 || b2) Log("bad"); return 9; }
    virtual void AddPersonToConversation(long c, const CScriptThing& p) { Log("person"); if (c != 9 || &p != &g_hero) Log("bad"); }
    virtual void AddLineToConversation(long c, const CCharString& t, bool b, const CScriptThing& s, const CScriptThing& l)
    { Log("line"); std::strcpy(text, t.Text); if (c != 9 || b || &s != speaker || &l != &g_hero) Log("bad"); }
};
static CGameScriptInterface g_global;
static CGameScriptInterface* g_pGameScriptInterface = &g_global;
struct CTimer { long Index; };
struct CScriptBase { int polls; bool IsActiveThreadTerminating() { ++polls; return false; } };
struct CQ_OrchardFarmRaidScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    CTimer CommentTimer;
    CCharString TextSystemScriptCode;
    CQ_OrchardFarmRaidScript() : TextSystemScriptCode("TEXT_OFR_") {}
    bool MakeTeamMemberComment(const CCharString& line, const CScriptThing& speaker, bool unused);
};
bool CQ_OrchardFarmRaidScript::MakeTeamMemberComment(const CCharString& line, const CScriptThing& speaker, bool unused)
{
    if (g_pGameScriptInterface->GetTimer(CommentTimer.Index) > 0) {
        IsActiveThreadTerminating();
        return false;
    }
    long conversation = m_pInterface->AddNewConversation(speaker, false, false);
    m_pInterface->AddPersonToConversation(conversation, m_pInterface->GetHero());
    m_pInterface->AddLineToConversation(conversation, TextSystemScriptCode + speaker.GetDataString() + "_" + line, false, speaker, m_pInterface->GetHero());
    g_pGameScriptInterface->SetTimer(CommentTimer.Index, 5);
    return true;
}
int main()
{
    int bad = 0;
    CGameScriptInterface gi;
    CQ_OrchardFarmRaidScript s; s.polls = 0; s.m_pInterface = &gi; s.CommentTimer.Index = 12;
    CScriptThing bob; bob.data = "BOB";
    CCharString line("03");
    int base = g_live;
    g_global.timerValue = 4; g_log[0] = 0;
    if (s.MakeTeamMemberComment(line, bob, true) || g_log[0] || s.polls != 1) bad = 1;
    g_global.timerValue = 0; g_log[0] = 0;
    if (!s.MakeTeamMemberComment(line, bob, true) || std::strcmp(g_log, "new;person;line;set;") != 0) bad = 1;
    if (std::strcmp(gi.text, "TEXT_OFR_BOB_03") != 0 || g_global.setTimer != 12 || g_global.setSecs != 5 || g_live != base) bad = 1;
    if (bad) { std::printf("00dcda80_TEST FAIL\n"); return 1; }
    std::printf("00dcda80_TEST PASS\n");
    return 0;
}