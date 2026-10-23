// 00e11450 behaviour test: CWaspAttacker::Main (match.cpp body verbatim, host stand-ins)
// shared host stand-ins for the I-44 behaviour tests (inlined into each test by gen.py)
#include <stdio.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static int g_calls = 0, g_termAt = 0, g_frames = 0;
static void L(const char* s) { g_log += s; g_log += ";"; }
struct CCharString {
    std::string S;
    CCharString(const char* t, long n = -1) : S(t) { (void)n; }
};
static CCharString operator+(const CCharString& a, const CCharString& b) { CCharString r(""); r.S = a.S + b.S; return r; }
struct CRGBColour { unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {} };
// a thing: Id, and how many IsAlive checks it survives
static int g_alive[8];
struct CScriptThing {
    int Id;
    CScriptThing() : Id(0) {}
    explicit CScriptThing(int id) : Id(id) {}
    virtual ~CScriptThing() {}
    virtual bool IsAlive() const { L("alive?"); if (g_alive[Id] > 0) { --g_alive[Id]; return true; } return false; }
    bool MsgIsHitBy(const char* by) const { std::string b(by); return (g_frames == 2 && Id == 2 && b == "") || (g_frames == 3 && Id == 3 && b == "CorneredGuard"); }
};
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};

static int g_scripted = 0;
struct CScriptGameResourceObjectScriptedThingBase {
    CScriptGameResourceObjectScriptedThingBase() { ++g_scripted; L("handle"); }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_scripted; L("~handle"); }
};
struct CGameScriptInterface {
    CScriptThing Hero;
    CGameScriptInterface() : Hero(1) {}
    void NewScriptFrame() const { ++g_frames; L("|"); }
    const CScriptThing& GetHero() const { return Hero; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const { CHECK(n.S == "WaspVictim"); L("victim"); return CScriptThing(3); }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& g) const { char b[16]; sprintf(b, "enemy%d>%d", t.Id, g.Id); L(b); }
};
struct CIsThingAlive { CScriptThing m_thing; CIsThingAlive(const CScriptThing& t) : m_thing(t) {} };
class CWaspAttacker {
public:
    CGameScriptInterface* Game; CScriptThing Me;
    void SetPredicate(CIsThingAlive& p) { CHECK(p.m_thing.Id == 2); L("pred"); }
    bool IsActiveThreadTerminating() { return g_termAt && ++g_calls >= g_termAt; }
    void Main();
};
void CWaspAttacker::Main()
{
    {
        CIsThingAlive predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    CScriptThing victim = Game->GetThingWithScriptName("WaspVictim");
    Game->GiveThingBestEnemyTarget(Me, victim);
    while (reinterpret_cast<const CScriptThingView&>(victim).Thing().IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
}

static std::string run(int termAt, int alive)
{
    CGameScriptInterface game; g_log = ""; g_calls = 0; g_termAt = termAt; g_frames = 0; g_alive[3] = alive;
    CWaspAttacker w; w.Game = &game; w.Me = CScriptThing(2);
    w.Main();
    CHECK(g_scripted == 0);
    return g_log;
}
int main()
{
    std::string full = run(0, 3);
    CHECK(full == "pred;|;handle;victim;enemy2>3;alive?;|;alive?;|;alive?;|;alive?;enemy2>1;~handle;");
    CHECK(run(0, 0) == "pred;|;handle;victim;enemy2>3;alive?;enemy2>1;~handle;");
    CHECK(run(1, 3) == "pred;|;");
    CHECK(run(2, 3) == "pred;|;handle;victim;enemy2>3;alive?;|;~handle;");
    CHECK(run(4, 3) == "pred;|;handle;victim;enemy2>3;alive?;|;alive?;|;alive?;|;~handle;");
    CHECK(run(5, 3) == "pred;|;handle;victim;enemy2>3;alive?;|;alive?;|;alive?;|;alive?;~handle;");
    if (g_fail) { printf("FAILED %s\n", full.c_str()); return 1; }
    printf("00e11450_TEST PASS\n");
    return 0;
}