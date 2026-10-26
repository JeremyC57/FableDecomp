// Behaviour test for CTCHeroOnlineScoreboard::AbortWithMessage (0x00563410): the looked-up text is
// shown, the scoreboard cleared, the states set, and an open connection closed.
#include <cstdio>

static int g_events[8];
static int g_count = 0;
static int g_shownText = 0;
static int g_liveStrings = 0;

class CCharString { public:
    int m_symbol;
};

class CWideString { public:
    int m_text;
    CWideString(int text) : m_text(text) { ++g_liveStrings; }
    CWideString(const CWideString& other) : m_text(other.m_text) { ++g_liveStrings; }
    ~CWideString() { --g_liveStrings; }
};

struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol) { return CWideString(symbol.m_symbol * 10); }
};

struct CEngineGlobals {
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;
};

static CDataBank g_bank;
static CEngineGlobals g_globals;
static CEngineGlobals* g_pEngineGlobals_013b86a0 = &g_globals;

namespace LHNetworkLib {
class LHHttp2 { public:
    int m_closed;
    void Close() { ++m_closed; g_events[g_count++] = 3; }
};
}

struct CHttpScopedPtr {
    LHNetworkLib::LHHttp2* px;
    typedef LHNetworkLib::LHHttp2* (CHttpScopedPtr::*unspecified_bool_type)() const;
    LHNetworkLib::LHHttp2* get() const { return px; }
    operator unspecified_bool_type() const { return px == 0 ? 0 : &CHttpScopedPtr::get; }
    LHNetworkLib::LHHttp2* operator->() const { return px; }
};

struct CComponent;

class CTCHeroOnlineScoreboard { public:
    virtual void v00() {}
    void*          m_pThing;
    long           m_8;
    CHttpScopedPtr PHttp;
    unsigned char  _scoreboardData[0x40];
    long           ScoreboardState;
    long           TaskState;
    CComponent*    PScoreboardScreen;
    CComponent*    PClanMenuEntry;
    CComponent*    PConnectingMessage;
    CComponent*    PConnectingMessage1;
    void ShowMessage(const CWideString& text) { g_shownText = text.m_text; g_events[g_count++] = 1; }
    void ClearScoreboard() { g_events[g_count++] = 2; }
    void AbortWithMessage(const CCharString& textSymbol);
};

void CTCHeroOnlineScoreboard::AbortWithMessage(const CCharString& textSymbol)
{
    CWideString text = g_pEngineGlobals_013b86a0->m_pTextBank->GetTextBySymbol(textSymbol);
    ShowMessage(text);
    ClearScoreboard();
    ScoreboardState = 0;
    TaskState = 100;
    if (PHttp)
        PHttp->Close();
}

int main()
{
    g_globals.m_pTextBank = &g_bank;
    LHNetworkLib::LHHttp2 http;
    http.m_closed = 0;
    CTCHeroOnlineScoreboard s;
    s.PHttp.px = &http;
    s.ScoreboardState = 4; s.TaskState = 1;
    CCharString symbol; symbol.m_symbol = 7;
    s.AbortWithMessage(symbol);
    bool ok = g_shownText == 70 && g_count == 3 && g_events[0] == 1 && g_events[1] == 2 && g_events[2] == 3
        && http.m_closed == 1 && s.ScoreboardState == 0 && s.TaskState == 100 && g_liveStrings == 0;
    s.PHttp.px = 0;
    symbol.m_symbol = 2;
    s.AbortWithMessage(symbol);
    ok = ok && g_shownText == 20 && g_count == 5 && http.m_closed == 1 && g_liveStrings == 0;
    if (!ok) {
        std::printf("00563410_TEST FAIL\n");
        return 1;
    }
    std::printf("00563410_TEST PASS\n");
    return 0;
}