// Behaviour test for 0x00DDC070 NScript::CQ_PrisonRaceScript::CPrisonRaceWinningPostGuard::Main(): the row's body copied over
// test doubles (handle, game interface, parent quest flag, thread-termination counter); checks the call order of every exit.
#include <cstdio>
#include <cstring>
static int g_log[256]; static int g_n = 0;
static void Log(int v) { if (g_n < 256) g_log[g_n++] = v; }
class CCharString { public: const char* s; CCharString(const char* t) : s(t) { } };
class CScriptThing { public: unsigned char body[12]; float GetAngleXY() const { Log(20); return 1.5f; } };
static bool g_valid = true;
static int g_performFrames = 0;
class CScriptGameResourceObjectScriptedThingBase { public:
    CScriptGameResourceObjectScriptedThingBase() { Log(1); }
    ~CScriptGameResourceObjectScriptedThingBase() { Log(2); }
    bool IsValid() const { Log(3); return g_valid; }
    void Release() { Log(4); }
    void PlayAnimation(const CCharString& a, bool, bool, bool, bool e, bool, bool, bool) { Log(std::strcmp(a.s, "ST_SALUTE") == 0 && e ? 30 : 99); }
    bool IsPerformingScriptTask() const { Log(31); return g_performFrames-- > 0; }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
static int g_startFailures = 0;
static bool g_physicsDefault = false;
class CGame { public:
    void NewScriptFrame() const { Log(5); }
    bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority p) { Log(p == 4 ? 6 : 99); if (g_startFailures > 0) { --g_startFailures; return false; } return true; }
    void EntitySheatheWeapons(const CScriptThing&, bool s) const { Log(s ? 99 : 10); }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString& n) const { Log(std::strcmp(n.s, "GuardPosition") == 0 ? 11 : 99); CScriptThing t; std::memset(t.body, 0, 12); return t; }
    void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool b) const { Log(b ? 99 : 12); }
    void EntitySetFacingAngle(const CScriptThing&, float a, bool b) const { Log(a == 1.5f && b ? 13 : 99); }
};
struct CQ_PrisonRaceScript { bool HeroWonRace; };
static int g_doneAfter = 0, g_doneCalls = 0, g_wonAfter = 0, g_wonCalls = 0;
static CQ_PrisonRaceScript g_parent;
struct CPrisonRaceWinningPostGuard {
    CGame* Game; CScriptThing Me; CQ_PrisonRaceScript& ParentClass;
    CPrisonRaceWinningPostGuard() : ParentClass(g_parent) { }
    bool IsActiveThreadTerminating() const { Log(9); g_parent.HeroWonRace = ++g_wonCalls >= g_wonAfter; return ++g_doneCalls >= g_doneAfter; }
    void Main();
};
#define NGlobalConsole_GDefault g_physicsDefault
void CPrisonRaceWinningPostGuard::Main()
{
    CScriptGameResourceObjectScriptedThingBase guard;
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySheatheWeapons(Me, false);
    Game->EntityTeleportToThing(Me, Game->GetNearestWithScriptName(Me, "GuardPosition"), false);
    Game->EntitySetFacingAngle(Me, Game->GetNearestWithScriptName(Me, "GuardPosition").GetAngleXY(), true);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass.HeroWonRace)
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    guard.PlayAnimation("ST_SALUTE", false, false, false, true, NGlobalConsole_GDefault, false, false);
    while (guard.IsPerformingScriptTask()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
static bool Run(bool valid, int failures, int doneAfter, int wonAfter, int perform, const int* want, int nwant)
{
    CGame game; CPrisonRaceWinningPostGuard g; g.Game = &game; std::memset(g.Me.body, 0, 12);
    g_n = 0; g_valid = valid; g_startFailures = failures; g_doneAfter = doneAfter; g_doneCalls = 0;
    g_wonAfter = wonAfter; g_wonCalls = 0; g_performFrames = perform; g_parent.HeroWonRace = false;
    g.Main();
    if (g_n != nwant) { for (int i = 0; i < g_n; ++i) std::printf("%d ", g_log[i]); std::printf("\n"); return false; }
    for (int i = 0; i < nwant; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
int main()
{
    // terminating right after the handle starts
    static const int a[] = { 1, 3, 4, 6, 9, 2 };
    // one failed start, terminating inside the start wait
    static const int b[] = { 1, 3, 6, 5, 9, 2 };
    // set up, hero has not won: one frame, then terminating in the race wait
    static const int c[] = { 1, 3, 4, 6, 9, 10, 11, 12, 11, 20, 13, 9, 5, 9, 2 };
    // hero won at the first check: salute, one performing frame, then three idle frames
    static const int d[] = { 1, 3, 4, 6, 9, 10, 11, 12, 11, 20, 13, 9, 9, 30, 31, 5, 9, 31, 9, 5, 9, 5, 9, 5, 9, 2 };
    bool ok = Run(true, 0, 1, 99, 0, a, 6) && Run(false, 1, 1, 99, 0, b, 6)
        && Run(true, 0, 3, 99, 0, c, 15) && Run(true, 0, 8, 2, 1, d, 26);
    if (!ok) { std::printf("00ddc070_TEST FAIL\n"); return 1; }
    std::printf("00ddc070_TEST PASS\n");
    return 0;
}