// Behaviour test for 0x00D91960 NScript::CQ_HobbeCaveScript::CHobbeRoaming::Main(): the row's body copied over test doubles
// (predicate, handle, game interface, parent quest flags, rand, thread-termination counter); checks the call order of each path.
#include <cstdio>
#include <cstring>
#include <vector>
namespace hobbe_test {   // the doubles shadow ::rand for the copied body
static int g_log[512]; static int g_n = 0;
static void Log(int v) { if (g_n < 512) g_log[g_n++] = v; }
struct CCharString { const char* s; CCharString(const char* t) : s(t) { } };
static bool g_aware = false;
static int g_aliveFrames = 0;
struct CScriptThing {
    int id;
    CScriptThing() : id(0) { }
    bool IsAwareOfHero() const { Log(40); return g_aware; }
    bool IsAlive() const { Log(41); return g_aliveFrames-- > 0; }
};
struct CIsThingAlive { CScriptThing t; CIsThingAlive(const CScriptThing& x) : t(x) { Log(50); } ~CIsThingAlive() { Log(51); } };
static int g_rand[8]; static int g_randN = 0;
static int rand() { Log(60); return g_rand[g_randN++]; }
static bool g_near = false;
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing& b, float d) { Log(d == 15.0f && b.id == 7 ? 61 : (d == 10.0f && b.id == 9 ? 62 : 99)); return g_near; }
struct CScriptGameResourceObjectScriptedThingBase {
    CScriptGameResourceObjectScriptedThingBase() { Log(1); }
    ~CScriptGameResourceObjectScriptedThingBase() { Log(2); }
    bool IsValid() const { Log(3); return false; }
    void Release() { Log(4); }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGH = 3 };
static int g_followers = 0; static bool g_regionLoaded = false;
struct CGame {
    CScriptThing hero;
    void NewScriptFrame() const { Log(5); }
    bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority p) { Log(p == 3 ? 6 : 99); return true; }
    CScriptThing& GetHero() { hero.id = 7; return hero; }
    void PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString& s) const { Log(std::strcmp(s.s, "HOBBE_ALARM_CALL") == 0 ? 10 : 99); }
    void GetFollowingEntityList(const CScriptThing& h, std::vector<CScriptThing>& f) const { Log(h.id == 7 ? 11 : 99); for (int i = 0; i < g_followers; ++i) { CScriptThing t; t.id = 20 + i; f.push_back(t); } }
    bool IsRegionLoaded(const CCharString& s) const { Log(std::strcmp(s.s, "HobbeCaveNymphFocal") == 0 ? 12 : 99); return g_regionLoaded; }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& t) const { Log(100 + t.id); }
    CScriptThing GetThingWithScriptName(const CCharString& s) const { Log(std::strcmp(s.s, "BanditSacrifice") == 0 ? 13 : 99); CScriptThing t; t.id = 9; return t; }
};
struct CQ_HobbeCaveScript { bool AlarmTriggered; bool HobbesCloseby; };
static int g_doneAfter = 0, g_doneCalls = 0;
struct CHobbeRoaming {
    CGame* Game; CScriptThing Me; CQ_HobbeCaveScript* ParentClass;
    void SetPredicate(const CIsThingAlive&) { Log(52); }
    bool IsActiveThreadTerminating() const { Log(9); return ++g_doneCalls >= g_doneAfter; }
    void Main();
};
void CHobbeRoaming::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase hobbe;
    if (hobbe.IsValid())
        hobbe.Release();
    while (!Game->StartScriptingEntity(Me, hobbe, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->AlarmTriggered) {
        if (IsActiveThreadTerminating())
            return;
        if (rand() % 4 == 0) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
            for (;;) {
                if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f) || Me.IsAwareOfHero())
                    break;
                Game->NewScriptFrame();
                if (!IsActiveThreadTerminating())
                    continue;
                return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->PlayCriteriaSoundOnThing(Me, "HOBBE_ALARM_CALL");
            if (IsActiveThreadTerminating())
                return;
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        while (!ParentClass->AlarmTriggered) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Me.IsAwareOfHero()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->PlayCriteriaSoundOnThing(Me, "HOBBE_ALARM_CALL");
                ParentClass->AlarmTriggered = true;
            }
        }
        if (IsActiveThreadTerminating())
            return;
    }
    {
        std::vector<CScriptThing> followers;
        Game->GetFollowingEntityList(Game->GetHero(), followers);
        if (followers.size() != 0 && rand() % 3 == 0 && !Game->IsRegionLoaded("HobbeCaveNymphFocal")) {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, followers[rand() % followers.size()]);
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        }
    }
    hobbe.Release();
    CScriptThing sacrifice = Game->GetThingWithScriptName("BanditSacrifice");
    while (sacrifice.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!ParentClass->HobbesCloseby && IsDistanceBetweenThingsUnder(Me, sacrifice, 10.0f)) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->HobbesCloseby = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
static bool Run(bool alarm, int r0, int r1, int r2, int followers, int doneAfter, int alive, const int* want, int nwant, bool* closeby)
{
    CGame game; CQ_HobbeCaveScript parent; parent.AlarmTriggered = alarm; parent.HobbesCloseby = false;
    CHobbeRoaming h; h.Game = &game; h.ParentClass = &parent;
    g_n = 0; g_rand[0] = r0; g_rand[1] = r1; g_rand[2] = r2; g_randN = 0; g_followers = followers; g_regionLoaded = false;
    g_doneAfter = doneAfter; g_doneCalls = 0; g_aliveFrames = alive; g_aware = true; g_near = true;
    h.Main();
    if (closeby) *closeby = parent.HobbesCloseby;
    if (g_n != nwant) { for (int i = 0; i < g_n; ++i) std::printf("%d ", g_log[i]); std::printf("\n"); return false; }
    for (int i = 0; i < nwant; ++i) if (g_log[i] != want[i]) { for (int k = 0; k < g_n; ++k) std::printf("%d ", g_log[k]); std::printf("\n"); return false; }
    return true;
}
}
using hobbe_test::Run;
int main()
{
    // terminating right after the predicate frame
    static const int a[] = { 50, 52, 51, 5, 9 };
    // alarm raised, rand()%4 != 0, no followers: attack the hero; sacrifice alive one frame (flag set), then three idle frames
    static const int b[] = { 50, 52, 51, 5, 9, 1, 3, 6, 9, 9, 60, 11, 9, 107, 4, 13, 41, 5, 9, 62, 9, 41, 9, 5, 9, 5, 9, 5, 9, 2 };
    // alarm raised, rand()%4 == 0: hero near -> alarm call; one follower and rand()%3 == 0, region not loaded: attack follower 0; the sacrifice is dead: five idle frames
    static const int c[] = { 50, 52, 51, 5, 9, 1, 3, 6, 9, 9, 60, 9, 9, 61, 9, 10, 9, 11, 60, 12, 9, 60, 120, 4, 13, 41, 9, 5, 9, 5, 9, 5, 9, 5, 9, 5, 9, 2 };
    bool cb = false;
    bool ok = Run(false, 0, 0, 0, 0, 1, 0, a, 5, 0)
        && Run(true, 1, 0, 0, 0, 10, 1, b, 30, &cb) && cb
        && Run(true, 4, 3, 0, 1, 14, 0, c, 38, 0);
    if (!ok) { std::printf("00d91960_TEST FAIL\n"); return 1; }
    std::printf("00d91960_TEST PASS\n");
    return 0;
}