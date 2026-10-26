// Behaviour test for CQ_SummoningTheShipScript::MakeBriarRoseComment (0x00DF20F0): no comment while a conversation is
// active; otherwise the commentary timer is reset to 15, and only a living Briar Rose gets a new conversation with the
// hero added and the line "TEXT_QST_B02_BRIARROSE_<line>" queued; strings are released on every path.
#include <cstdio>
#include <cstring>

static int g_live = 0, g_thingDtors = 0;
class CCharString {
public:
    char Text[64];
    CCharString(const char* text, long = -1) { std::strcpy(Text, text); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
};
static CCharString operator+(const char* l, const CCharString& r) { char b[64]; std::strcpy(b, l); std::strcat(b, r.Text); return CCharString(b); }
class CBaseClass { public: CBaseClass() {} virtual ~CBaseClass() {} };
class CScriptThing : public CBaseClass { public:
    CScriptThing* PImp_Data; bool alive;
    CScriptThing() : PImp_Data(0), alive(false) {}
    CScriptThing(const CScriptThing& o) : CBaseClass(), PImp_Data(o.PImp_Data), alive(o.alive) {}
    ~CScriptThing() { ++g_thingDtors; }
    virtual bool IsAlive() const { return PImp_Data && PImp_Data->alive; }
};
static CScriptThing g_hero, g_briarGame;
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CGameScriptInterface { public:
    bool active; long timer, secs; char text[64]; long convAsked;
    virtual CScriptThing& GetHero() { return g_hero; }
    virtual CScriptThing GetThingWithScriptName(const CCharString& n) { Log(n.Text); CScriptThing t; t.PImp_Data = &g_briarGame; return t; }
    virtual void SetTimer(long t, long s) { Log("timer"); timer = t; secs = s; }
    virtual long AddNewConversation(const CScriptThing& s, bool b1, bool b2) { Log("new"); if (b1 || b2 || s.PImp_Data != &g_briarGame) Log("bad"); return 77; }
    virtual void AddPersonToConversation(long c, const CScriptThing& p) { Log("person"); if (c != 77 || &p != &g_hero) Log("bad"); }
    virtual void AddLineToConversation(long c, const CCharString& t, bool b, const CScriptThing& s, const CScriptThing& l)
    { Log("line"); std::strcpy(text, t.Text); if (c != 77 || b || s.PImp_Data != &g_briarGame || &l != &g_hero) Log("bad"); }
    virtual bool IsConversationActive(long c) { convAsked = c; return active; }
};
static CGameScriptInterface g_global;
static CGameScriptInterface* g_pGameScriptInterface = &g_global;
class CTimer { public: long Index; };
struct CQ_SummoningTheShipScript {
    CGameScriptInterface* m_pInterface;
    CTimer CommentaryTimer;
    long ConversationIndex;
    bool MakeBriarRoseComment(const CCharString& line);
};
bool CQ_SummoningTheShipScript::MakeBriarRoseComment(const CCharString& line)
{
    if (m_pInterface->IsConversationActive(ConversationIndex))
        return false;
    g_pGameScriptInterface->SetTimer(CommentaryTimer.Index, 15);
    CScriptThing briar = m_pInterface->GetThingWithScriptName("STS_BriarRose");
    if (!briar.IsAlive())
        return false;
    long conversation = m_pInterface->AddNewConversation(briar, false, false);
    ConversationIndex = conversation;
    m_pInterface->AddPersonToConversation(conversation, m_pInterface->GetHero());
    m_pInterface->AddLineToConversation(conversation, "TEXT_QST_B02_BRIARROSE_" + line, false, briar, m_pInterface->GetHero());
    return true;
}
int main()
{
    int bad = 0;
    CGameScriptInterface gi; gi.text[0] = 0;
    CQ_SummoningTheShipScript s; s.m_pInterface = &gi; s.CommentaryTimer.Index = 5; s.ConversationIndex = 3;
    CCharString line("04");
    // active conversation: nothing happens
    gi.active = true; g_log[0] = 0; g_live = 1;
    if (s.MakeBriarRoseComment(line) || g_log[0] || gi.convAsked != 3) bad = 1;
    // Briar dead: timer reset only
    gi.active = false; g_briarGame.alive = false; g_global.timer = 0; g_log[0] = 0;
    if (s.MakeBriarRoseComment(line) || std::strcmp(g_log, "timer;STS_BriarRose;") != 0 || g_global.timer != 5 || g_global.secs != 15) bad = 1;
    // Briar alive: conversation built
    g_briarGame.alive = true; g_log[0] = 0;
    if (!s.MakeBriarRoseComment(line) || std::strcmp(g_log, "timer;STS_BriarRose;new;person;line;") != 0) bad = 1;
    if (s.ConversationIndex != 77 || std::strcmp(gi.text, "TEXT_QST_B02_BRIARROSE_04") != 0 || g_live != 1) bad = 1;
    if (bad) { std::printf("00df20f0_TEST FAIL\n"); return 1; }
    std::printf("00df20f0_TEST PASS\n");
    return 0;
}