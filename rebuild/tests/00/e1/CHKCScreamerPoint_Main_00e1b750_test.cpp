// 00e1b750 behaviour test: CHKC_ScreamerPoint::Main (match.cpp body verbatim, host stand-ins)
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

struct C3DVectorH { float X, Y, Z; };
static int g_handles = 0, g_armAt = 0, g_stopAt = 0, g_nearMask = 0, g_spawned = 0, g_cur = 0;
struct CScriptGameResourceObjectScriptedThingBase {
    CScriptGameResourceObjectScriptedThingBase() { ++g_handles; L("handle"); }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_handles; L("~handle"); }
};
struct CHKCMasterData { bool ScreamersArmed; bool ScreamersStopped; };
static CHKCMasterData* g_md;
struct CGameScriptInterface {
    CScriptThing Hero;
    CGameScriptInterface() : Hero(1) {}
    void NewScriptFrame() const {
        ++g_frames; L("|");
        if (g_frames >= g_armAt) g_md->ScreamersArmed = true;
        if (g_frames >= g_stopAt) g_md->ScreamersStopped = true;
        if (g_cur && g_alive[g_cur] > 0) --g_alive[g_cur];
    }
    const CScriptThing& GetHero() const { return Hero; }
    CScriptThing CreateCreature(const CCharString& d, const C3DVectorH& p, const CCharString& s, bool u) const {
        CHECK(d.S == "CREATURE_SCREAMER_01" && s.S == "SpawnedScreamer" && u && p.X == 2.0f);
        ++g_spawned; L("create"); g_alive[4] = 3; return CScriptThing(4);
    }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& g) const { char b[16]; sprintf(b, "enemy%d>%d", t.Id, g.Id); L(b); g_cur = t.Id; }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { CHECK(a.Id == 1 && b.Id == 2 && d == 20.0f); return ((g_nearMask >> g_frames) & 1) != 0; }
struct CIsThingAlive { CScriptThing m_thing; CIsThingAlive(const CScriptThing& t) : m_thing(t) {} };
static C3DVectorH g_mePos = { 2.0f, 0, 0 };
struct CMe : CScriptThing { CMe() : CScriptThing(2) {} const C3DVectorH& GetPos() const { return g_mePos; } };
struct CEntityScriptBase {
    CGameScriptInterface* Game; CMe Me;
    void SetPredicate(CIsThingAlive& p) { CHECK(p.m_thing.Id == 2); L("pred"); }
    bool IsActiveThreadTerminating() const { L("t?"); return g_termAt && ++g_calls >= g_termAt; }
};
inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }
struct CHKC_ScreamerPoint : CEntityScriptBase {
    CHKCMasterData* PMasterData; long SpawnTimer;
    void Main();
};
void CHKC_ScreamerPoint::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    while (!PMasterData->ScreamersArmed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!PMasterData->ScreamersStopped) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 20.0f)) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing screamer = Game->CreateCreature("CREATURE_SCREAMER_01", Me.GetPos(), "SpawnedScreamer", true);
            SpawnTimer = 0;
            Game->GiveThingBestEnemyTarget(screamer, Game->GetHero());
            while (reinterpret_cast<const CScriptThingView&>(screamer).Thing().IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    IsActiveThreadTerminating();
}

static std::string run(int termAt, int nearMask)
{
    CGameScriptInterface game; CHKCMasterData md = { false, false }; g_md = &md;
    g_log = ""; g_calls = 0; g_termAt = termAt; g_frames = 0; g_armAt = 2; g_stopAt = 9; g_nearMask = nearMask; g_spawned = 0; g_cur = 0;
    CHKC_ScreamerPoint p; p.Game = &game; p.PMasterData = &md; p.SpawnTimer = 5;
    p.Main();
    CHECK(g_handles == 0);
    if (g_spawned) CHECK(p.SpawnTimer == 0);
    return g_log;
}
int main()
{
    // hero near at frame 3: spawn, wait 3 frames for the screamer to die, then idle until stopped at frame 9
    std::string full = run(0, 1 << 3);
    CHECK(full == "pred;|;t?;handle;|;t?;t?;|;t?;t?;create;enemy4>1;alive?;|;t?;alive?;|;t?;alive?;t?;|;t?;|;t?;|;t?;|;t?;t?;~handle;");
    CHECK(run(0, 0).find("create") == std::string::npos);
    for (int n = 1; n < 30; ++n) {
        std::string s = run(n, 1 << 3);
        if (g_calls < n) { CHECK(s == full); break; }
        std::string pre = s.size() >= 8 && s.compare(s.size() - 8, 8, "~handle;") == 0 ? s.substr(0, s.size() - 8) : s;
        CHECK(full.compare(0, pre.size(), pre) == 0);
    }
    if (g_fail) { printf("FAILED %s\n", full.c_str()); return 1; }
    printf("00e1b750_TEST PASS\n");
    return 0;
}