// Behaviour test for CTE_BanditLeader::Main (0x00E035E0): after registering the filter the script waits (one frame per
// failed test) until Me is within 15 of the hero, opens a conversation on Me, adds the hero, adds the bandit-attack
// line from Me to the hero, targets the hero, then yields until done; a done script stops inside the wait.
#include <cstdio>
#include <cstring>

struct CScriptThing { int id; };
struct CCharString {
    const char* text;
    CCharString(const char* t, long = -1) : text(t) {}
};
static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
static int g_frame = 0, g_doneAt = 0, g_nearAt = 0;
static CScriptThing g_hero = { 7 };
static bool g_lineOk = false;

struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; Log(1); }
    CScriptThing& GetHero() { return g_hero; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { Log(t.id == 3 && !a && !b ? 364 : 0); return 42; }
    void AddPersonToConversation(long c, const CScriptThing& t) { Log(c == 42 && t.id == 7 ? 365 : 0); }
    void AddLineToConversation(long c, const CCharString& s, bool f, const CScriptThing& a, const CScriptThing& b)
    {
        g_lineOk = c == 42 && std::strcmp(s.text, "TEXT_QST_067_BANDIT_ATTACK") == 0 && !f && a.id == 3 && b.id == 7;
        Log(366);
    }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& target) { Log(t.id == 3 && target.id == 7 ? 451 : 0); }
};
bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d)
{ return a.id == 3 && b.id == 7 && d == 15.0f && g_frame >= g_nearAt; }

struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { Log(2); }
};

class CTE_BanditLeader {
public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    void RegisterFilter(CIsThingAlive& f) { Log(f.m_thing.id == 3 ? 3 : 0); }
    bool IsDone() { return g_frame >= g_doneAt; }
    void Main();
};

void CTE_BanditLeader::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    {
        CCharString text("TEXT_QST_067_BANDIT_ATTACK");
        Game->AddLineToConversation(conversation, text, false, Me, Game->GetHero());
    }
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}

int main()
{
    CGameScriptInterface game;
    CTE_BanditLeader s; s.Game = &game; s.Me.id = 3;
    g_nearAt = 3; g_doneAt = 6;
    s.Main();
    static const int e1[] = { 3, 2, 1, 1, 1, 364, 365, 366, 451, 1, 1, 1 };
    int ok = g_n == 12 && g_lineOk;
    for (int i = 0; ok && i < 12; ++i) if (g_log[i] != e1[i]) ok = 0;
    g_n = 0; g_frame = 0; g_nearAt = 10; g_doneAt = 2;
    s.Main();
    static const int e2[] = { 3, 2, 1, 1 };
    ok = ok && g_n == 4;
    for (int i = 0; ok && i < 4; ++i) if (g_log[i] != e2[i]) ok = 0;
    if (!ok) { std::printf("00e035e0_TEST FAIL\n"); return 1; }
    std::printf("00e035e0_TEST PASS\n");
    return 0;
}
