// Behaviour test for CTCHeroOnlineScoreboard::ResetScoreboard (0x00562E70): the connection object
// is destroyed and freed, the scoreboard cleared, the state and screen pointers zeroed.
#include <cstdio>

static int g_events[8];
static int g_count = 0;

namespace LHNetworkLib {
struct LHHttp2 {
    ~LHHttp2() { g_events[g_count++] = 1; }
};
}

struct CHttpScopedPtr {
    LHNetworkLib::LHHttp2* px;
    void reset(LHNetworkLib::LHHttp2* p = 0)
    {
        LHNetworkLib::LHHttp2* old = px;
        px = p;
        delete old;
    }
};

struct CComponent { int dummy; };

struct CTCHeroOnlineScoreboard {
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
    void ClearScoreboard() { g_events[g_count++] = 2; }
    void ResetScoreboard();
};

void CTCHeroOnlineScoreboard::ResetScoreboard()
{
    PHttp.reset();
    ClearScoreboard();
    ScoreboardState = 0;
    PConnectingMessage = 0;
    PConnectingMessage1 = 0;
    PScoreboardScreen = 0;
}

int main()
{
    static CComponent a, b, c, d;
    CTCHeroOnlineScoreboard s;
    s.PHttp.px = new LHNetworkLib::LHHttp2;
    s.ScoreboardState = 3; s.TaskState = 7;
    s.PScoreboardScreen = &a; s.PClanMenuEntry = &b; s.PConnectingMessage = &c; s.PConnectingMessage1 = &d;
    s.ResetScoreboard();
    bool ok = g_count == 2 && g_events[0] == 1 && g_events[1] == 2 && s.PHttp.px == 0 && s.ScoreboardState == 0
        && s.TaskState == 7 && s.PScoreboardScreen == 0 && s.PClanMenuEntry == &b && s.PConnectingMessage == 0
        && s.PConnectingMessage1 == 0;
    s.ResetScoreboard();
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("00562e70_TEST FAIL\n");
        return 1;
    }
    std::printf("00562e70_TEST PASS\n");
    return 0;
}
