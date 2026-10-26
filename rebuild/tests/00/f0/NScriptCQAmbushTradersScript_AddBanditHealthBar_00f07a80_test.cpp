// Behaviour test for CQ_AmbushTradersScript::AddBanditHealthBar (0x00F07A80): the first bar shows the quest info and
// uses the plain icon; later bars use "<icon>_0<count+1>", past six a random 2..6; the bar is green and opaque at
// scale 1; the count goes up; a terminating thread returns -1 and changes nothing.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString {
public:
    char Text[64];
    CCharString(const char* t, long = -1) { std::strcpy(Text, t); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator+=(const char* t) { std::strcat(Text, t); return *this; }
    CCharString& operator+=(const CCharString& o) { std::strcat(Text, o.Text); return *this; }
};
static CCharString GFIntToCharString(long v) { char b[16]; std::sprintf(b, "%ld", v); return CCharString(b); }
static int g_rand = 0;
static int test_rand() { return g_rand; }
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CScriptThing { public: int id; };
class CGameScriptInterface { public:
    bool shown; char icon[64]; unsigned char r, g, b, a; float scale; const CScriptThing* who;
    virtual void DisplayQuestInfo(bool s) { shown = s; }
    virtual long AddQuestInfoBarHealth(const CScriptThing& t, const CRGBColour& c, const CCharString& i, float sc)
    { who = &t; r = c.R; g = c.G; b = c.B; a = c.A; std::strcpy(icon, i.Text); scale = sc; return 1000 + t.id; }
};
class CScriptBase { public: bool term; bool IsActiveThreadTerminating() { return term; } };
struct CQ_AmbushTradersScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    long HiredBanditCount;
    long AddBanditHealthBar(const CScriptThing& bandit);
};
long CQ_AmbushTradersScript::AddBanditHealthBar(const CScriptThing& bandit)
{
    CCharString icon("HUD_QUEST_ICON_BANDIT");
    if (HiredBanditCount > 0) {
        if (IsActiveThreadTerminating())
            return -1;
        icon += "_0";
        long number = HiredBanditCount + 1;
        if (number > 6) {
            if (IsActiveThreadTerminating())
                return -1;
            number = test_rand() % 5 + 2;
        }
        icon += GFIntToCharString(number);
    } else {
        if (IsActiveThreadTerminating())
            return -1;
        m_pInterface->DisplayQuestInfo(true);
    }
    long id = m_pInterface->AddQuestInfoBarHealth(bandit, CRGBColour(0, 255, 0, 255), icon, 1.0f);
    ++HiredBanditCount;
    return id;
}
int main()
{
    int bad = 0;
    CGameScriptInterface gi; gi.shown = false;
    CQ_AmbushTradersScript s; s.term = false; s.m_pInterface = &gi; s.HiredBanditCount = 0;
    CScriptThing t; t.id = 7;
    if (s.AddBanditHealthBar(t) != 1007 || !gi.shown || std::strcmp(gi.icon, "HUD_QUEST_ICON_BANDIT") != 0 || s.HiredBanditCount != 1) bad = 1;
    if (gi.r != 0 || gi.g != 255 || gi.b != 0 || gi.a != 255 || gi.scale != 1.0f || gi.who != &t) bad = 1;
    gi.shown = false;
    if (s.AddBanditHealthBar(t) != 1007 || gi.shown || std::strcmp(gi.icon, "HUD_QUEST_ICON_BANDIT_02") != 0 || s.HiredBanditCount != 2) bad = 1;
    s.HiredBanditCount = 6; g_rand = 13;   // 13 % 5 + 2 = 5
    if (s.AddBanditHealthBar(t) != 1007 || std::strcmp(gi.icon, "HUD_QUEST_ICON_BANDIT_05") != 0 || s.HiredBanditCount != 7) bad = 1;
    s.term = true;
    if (s.AddBanditHealthBar(t) != -1 || s.HiredBanditCount != 7) bad = 1;
    s.HiredBanditCount = 0;
    if (s.AddBanditHealthBar(t) != -1 || s.HiredBanditCount != 0 || g_live != 0) bad = 1;
    if (bad) { std::printf("00f07a80_TEST FAIL\n"); return 1; }
    std::printf("00f07a80_TEST PASS\n");
    return 0;
}