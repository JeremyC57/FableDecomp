// 00d6aea0 behaviour test: CHTE_AssaultGuard::Main (match.cpp body verbatim, host stand-ins)
#include <stdio.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static int g_calls = 0, g_termAt = 0, g_frames = 0;
static void L(const char* s) { g_log += s; g_log += ";"; }
struct CCharString { std::string S; CCharString(const char* t, long n = -1) : S(t) { (void)n; } };
struct HostImp { int Id; int Hp; bool IsAlive() const { return Hp > 0; } };
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) {} };
static HostImp* g_cur = 0;
struct CScriptThing {
    CCountedPointerWords PImp;
    CScriptThing() {}
    explicit CScriptThing(HostImp* imp) { PImp.Data = imp; }
    virtual ~CScriptThing() {}
    int Id() const { return PImp.Data ? ((HostImp*)PImp.Data)->Id : 0; }
    virtual bool IsAlive() const { L("alive?"); return PImp.Data && ((HostImp*)PImp.Data)->IsAlive(); }
};
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};
static inline bool IsThingImpAlive(const CScriptThing& t) { HostImp* imp = (HostImp*)t.PImp.Data; L("impalive?"); return imp && imp->IsAlive(); }
namespace NScript {
struct CPredicateBase { virtual ~CPredicateBase() {} };
struct CIsThingAlive : CPredicateBase { CScriptThing Thing; CIsThingAlive(const CScriptThing& t) : Thing(t) {} };
}
static int g_scripting = 0, g_startAt = 0, g_farUntil = 0, g_taskBusy = 0, g_handles = 0;
struct CScriptGameResourceObjectScriptedThingBase {
    CScriptGameResourceObjectScriptedThingBase() { ++g_handles; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_handles; L("~handle"); }
    bool IsScripting() { return g_scripting != 0; }
    void Release() { L("release"); }
    bool IsPerformingScriptTask() const { return (g_frames & 1) == g_taskBusy; }
    void FollowThing(const CScriptThing& t, float d, bool run) { CHECK(t.Id() == 1 && d == 1.0f && run); L("followhero"); }
    void ClearCommands() { L("clear"); }
};
static HostImp g_hero = { 1, 100 }, g_me = { 2, 100 }, g_ff[3] = { { 3, 2 }, { 4, 1 }, { 5, 0 } };
static int g_nextFF = 0;
struct GameCalls {
    CScriptThing Hero;
    GameCalls() : Hero(&g_hero) {}
    void NewScriptFrame() { ++g_frames; L("|"); if (g_cur && g_cur->Hp > 0) --g_cur->Hp; }
    bool StartScriptingEntity(const CScriptThing& t, CScriptGameResourceObjectScriptedThingBase& h, long pri) { (void)h; CHECK(t.Id() == 2 && pri == 3); L("start?"); return g_frames >= g_startAt; }
    const CScriptThing& GetHero() const { return Hero; }
    void EntitySetThingAsEnemyOfThing(const CScriptThing& a, const CScriptThing& b) { CHECK(a.Id() == 2 && b.Id() == 1); L("enemyof"); }
    CScriptThing GetNearestWithScriptName(const CScriptThing& t, const CCharString& n) const { CHECK(t.Id() == 2 && n.S == "HTE_FreedomFighter"); L("nearest"); g_nextFF = 1; return CScriptThing(&g_ff[0]); }
    CScriptThing GetRandomThingWithScriptName(const CCharString& n) const { CHECK(n.S == "HTE_FreedomFighter"); L("random"); return g_nextFF < 3 ? CScriptThing(&g_ff[g_nextFF++]) : CScriptThing(); }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& g) const { char b[16]; sprintf(b, "enemy%d>%d", t.Id(), g.Id()); L(b); g_cur = (HostImp*)g.PImp.Data; }
};
static bool IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float d) { CHECK(d == 5.0f); (void)a; (void)b; return g_frames < g_farUntil; }
struct CHTE_AssaultGuard {
    void* VTable; GameCalls* Game; CScriptThing Me;
    void SetPredicate(const NScript::CPredicateBase& p) { CHECK(static_cast<const NScript::CIsThingAlive&>(p).Thing.Id() == 2); L("pred"); }
    bool IsActiveThreadTerminating() { return g_termAt && ++g_calls >= g_termAt; }
    void Main();
};
void CHTE_AssaultGuard::Main()
{
 SetPredicate(NScript::CIsThingAlive(Me));
 Game->NewScriptFrame();
 if (IsActiveThreadTerminating()) return;
 CScriptGameResourceObjectScriptedThingBase handle;
 if (handle.IsScripting())
  handle.Release();
 while (!Game->StartScriptingEntity(Me, handle, 3)) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
 }
 if (IsActiveThreadTerminating()) return;
 Game->EntitySetThingAsEnemyOfThing(Me, Game->GetHero());
 CScriptThing target = Game->GetNearestWithScriptName(Me, "HTE_FreedomFighter");
 while (IsDistanceBetweenThingsOver(Game->GetHero(), Me, 5.0f) && IsDistanceBetweenThingsOver(Me, target, 5.0f)) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
  if (!handle.IsPerformingScriptTask())
   handle.FollowThing(Game->GetHero(), 1.0f, true);
 }
 if (IsActiveThreadTerminating()) return;
 handle.ClearCommands();
 while (IsThingImpAlive(target)) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
  Game->GiveThingBestEnemyTarget(Me, target);
  while (reinterpret_cast<const CScriptThingView&>(target).Thing().IsAlive()) {
   Game->NewScriptFrame();
   if (IsActiveThreadTerminating()) return;
  }
  if (IsActiveThreadTerminating()) return;
  target.PImp = Game->GetRandomThingWithScriptName("HTE_FreedomFighter").PImp;
 }
 if (IsActiveThreadTerminating()) return;
 Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
}

static std::string run(int termAt, int scripting)
{
    GameCalls game; g_log = ""; g_calls = 0; g_termAt = termAt; g_frames = 0; g_cur = 0;
    g_scripting = scripting; g_startAt = 3; g_farUntil = 5; g_taskBusy = 0;
    g_ff[0].Hp = 2; g_ff[1].Hp = 1; g_ff[2].Hp = 0; g_nextFF = 0;
    CHTE_AssaultGuard a; a.VTable = 0; a.Game = &game; a.Me = CScriptThing(&g_me);
    a.Main();
    CHECK(g_handles == 0);
    return g_log;
}
int main()
{
    std::string full = run(0, 0);
    CHECK(full == "pred;|;start?;|;start?;|;start?;enemyof;nearest;|;|;followhero;clear;impalive?;|;enemy2>3;alive?;|;alive?;|;alive?;random;impalive?;|;enemy2>4;alive?;|;alive?;random;impalive?;enemy2>1;~handle;");
    CHECK(run(0, 1) == "pred;|;release;" + full.substr(7));
    CHECK(run(1, 0) == "pred;|;");
    // termination at each check: the log (minus the handle dtor) is a strict prefix of the full run, and never reaches the final hero target
    std::string body = full.substr(0, full.size() - 8);
    for (int n = 2; n < 40; ++n) {
        std::string s = run(n, 0);
        if (g_calls < n) { CHECK(s == full); break; }
        CHECK(s.size() >= 8 && s.compare(s.size() - 8, 8, "~handle;") == 0);
        s = s.substr(0, s.size() - 8);
        CHECK(body.compare(0, s.size(), s) == 0);
        CHECK(s.find("enemy2>1") == std::string::npos);
    }
    if (g_fail) { printf("FAILED %s\n", full.c_str()); return 1; }
    printf("00d6aea0_TEST PASS\n");
    return 0;
}