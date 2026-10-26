// 00dfc630 behaviour test: CQ_TraderConflictGoodScript::WatchForHittingEnemies (match.cpp body, operator[]
// as *(begin() + n)), host stand-ins. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const std::string& s) { g_log += s; g_log += ";"; }
static std::string num(int v) { char b[16]; sprintf(b, "%d", v); return b; }

struct CCharString {
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
enum EHeroAbility { HERO_ABILITY_NONE };

static int g_frame = 0;
struct CScriptThing {
    int Id;
    float X;
    int HitFrame;          // plainly hit by the hero on this frame
    int SpecialFrame;      // hit by a special ability on this frame
    int SpecialAbility;    // which ability
    bool AwareOfHero;
    bool MsgIsHitBy(const CCharString& attacker) const { return attacker.S == "" && HitFrame == g_frame; }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& attacker) const
    { return attacker.S == "SCRIPT_NAME_HERO" && SpecialFrame == g_frame && SpecialAbility == (int)ability; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& attacker) const { return attacker.S == "" && SpecialFrame == g_frame; }
};

static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{
    float d = a.X - b.X; if (d < 0) d = -d;
    return d < distance;
}

static int g_followAt = 1000, g_teleportStep = 0;
struct CGameScriptInterface {
    CScriptThing Hero;
    void NewScriptFrame() const { ++g_frame; }
    CScriptThing& GetHero() const { return const_cast<CScriptThing&>(Hero); }
    void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target)
    { logs("target " + num(thing.Id) + "->" + num(target.Id)); }
    bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing& thing, const CScriptThing& other) const
    { return thing.Id == Hero.Id && other.AwareOfHero; }
};

struct CScriptThingVector {
    std::vector<CScriptThing> V;
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    unsigned int size() const { return _M_finish - _M_start; }
    CScriptThing* begin() { return _M_start; }
    CScriptThing& operator[](unsigned int n) { return *(begin() + n); }
    void sync() { _M_start = V.empty() ? 0 : &V[0]; _M_finish = _M_start + V.size(); }
};

namespace NScript {
static int g_termCalls = 0, g_termAt = -1, g_termFrame = 100000;
struct CScriptBase {
    bool IsActiveThreadTerminating() { ++g_termCalls; return g_termCalls == g_termAt || g_frame >= g_termFrame; }
};

struct CQ_TraderConflictGoodScript : CScriptBase {
    CGameScriptInterface* GSI;
    void*                 PMasterData;
    CScriptThingVector    AllCreatures;
    long                  TradersFollowing;
    long                  TradersReachedTeleporter;
    void WatchForHittingEnemies();
    void UpdateTraders()
    {
        logs("update");
        if (g_frame >= g_followAt) TradersFollowing = 1;
        if (TradersFollowing >= 1 && g_frame > g_followAt) TradersReachedTeleporter += g_teleportStep;
    }
};

void CQ_TraderConflictGoodScript::WatchForHittingEnemies()
{
    while (TradersFollowing < 1) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        for (unsigned int i = 0; i < AllCreatures.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (AllCreatures[i].MsgIsHitBy("")
                || (AllCreatures[i].MsgIsHitByAnySpecialAbilityFrom("")
                    && !AllCreatures[i].MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return;
                for (unsigned int j = 0; j < AllCreatures.size(); ++j) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (IsDistanceBetweenThingsUnder(AllCreatures[i], AllCreatures[j], 20.0f)
                        || GSI->IsThingAwareOfOtherThingInAnyWay(GSI->GetHero(), AllCreatures[j])) {
                        if (IsActiveThreadTerminating())
                            return;
                        GSI->GiveThingBestEnemyTarget(AllCreatures[j], GSI->GetHero());
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        UpdateTraders();
    }
    if (IsActiveThreadTerminating())
        return;
    while (TradersReachedTeleporter < 3) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        UpdateTraders();
    }
    if (IsActiveThreadTerminating())
        return;
}
}

static CScriptThing thing(int id, float x)
{
    CScriptThing t; t.Id = id; t.X = x; t.HitFrame = -1; t.SpecialFrame = -1; t.SpecialAbility = 0; t.AwareOfHero = false;
    return t;
}
static void reset(NScript::CQ_TraderConflictGoodScript& s, int followAt, int step)
{
    g_log = ""; g_frame = 0; g_followAt = followAt; g_teleportStep = step;
    NScript::g_termCalls = 0; NScript::g_termAt = -1; NScript::g_termFrame = 100000;
    s.TradersFollowing = 0; s.TradersReachedTeleporter = 0;
    for (size_t k = 0; k < s.AllCreatures.V.size(); ++k) {
        s.AllCreatures.V[k].HitFrame = -1; s.AllCreatures.V[k].SpecialFrame = -1; s.AllCreatures.V[k].AwareOfHero = false;
    }
}
static int count(const std::string& s, const char* sub)
{
    int n = 0; size_t p = 0; std::string t(sub);
    while ((p = s.find(t, p)) != std::string::npos) { ++n; p += t.size(); }
    return n;
}

int main()
{
    CGameScriptInterface gsi; gsi.Hero = thing(99, 0.0f);
    NScript::CQ_TraderConflictGoodScript s; s.GSI = &gsi; s.PMasterData = 0;
    s.AllCreatures.V.push_back(thing(1, 0.0f));
    s.AllCreatures.V.push_back(thing(2, 10.0f));
    s.AllCreatures.V.push_back(thing(3, 50.0f));
    s.AllCreatures.V.push_back(thing(4, 200.0f));
    s.AllCreatures.sync();

    // no hits: one update per frame until a trader follows (frame 3), then until 3 reach the teleporter
    reset(s, 3, 1);
    s.WatchForHittingEnemies();
    CHECK(g_log == "update;update;update;update;update;update;");
    CHECK(s.TradersFollowing == 1 && s.TradersReachedTeleporter == 3 && g_frame == 6);

    // creature 2 plainly hit on frame 2: everything within 20 units of it (1, 2) turns on the hero;
    // creature 4 is aware of the hero and turns too; 3 is far and unaware
    reset(s, 3, 3);
    s.AllCreatures.V[1].HitFrame = 2; s.AllCreatures.V[3].AwareOfHero = true;
    s.WatchForHittingEnemies();
    CHECK(g_log == "update;target 1->99;target 2->99;target 4->99;update;update;update;");

    // any special-ability hit counts unless it is ability 14 from the hero
    reset(s, 2, 3);
    s.AllCreatures.V[2].SpecialFrame = 1; s.AllCreatures.V[2].SpecialAbility = 5;
    s.WatchForHittingEnemies();
    CHECK(g_log == "target 3->99;update;update;update;");
    reset(s, 2, 3);
    s.AllCreatures.V[2].SpecialFrame = 1; s.AllCreatures.V[2].SpecialAbility = 14;
    s.WatchForHittingEnemies();
    CHECK(count(g_log, "target") == 0);

    // two creatures hit on the same frame: each one's neighbourhood is processed in order
    reset(s, 1, 3);
    s.AllCreatures.V[0].HitFrame = 1; s.AllCreatures.V[3].HitFrame = 1;
    s.WatchForHittingEnemies();
    CHECK(g_log == "target 1->99;target 2->99;target 4->99;update;update;");

    // terminating mid-scan stops at once: no update, no further targets
    // calls on frame 1: 1 after NewScriptFrame, 2 i=0, 3 hit, 4 j=0, 5 before target 1, 6 j=1
    reset(s, 3, 1); s.AllCreatures.V[0].HitFrame = 1; NScript::g_termAt = 6;
    s.WatchForHittingEnemies();
    CHECK(g_log == "target 1->99;");

    // terminating in the teleporter phase
    reset(s, 1, 0); NScript::g_termFrame = 4;
    s.WatchForHittingEnemies();
    CHECK(s.TradersFollowing == 1 && s.TradersReachedTeleporter == 0 && g_frame == 4 && count(g_log, "update") == 3);

    // already following and already at the teleporter: nothing runs
    reset(s, 0, 0); s.TradersFollowing = 1; s.TradersReachedTeleporter = 3;
    s.WatchForHittingEnemies();
    CHECK(g_log == "" && g_frame == 0 && NScript::g_termCalls == 2);

    if (!g_fail) printf("00dfc630_TEST PASS\n");
    return g_fail;
}