// Behaviour test for 0x00D701C0 CHTG_FreedomFighter::Main (RST2): engine stubbed, the body verbatim from final.cpp.
// Checks: the CIsThingAlive(Me) predicate is installed; until the bandits flee the freedom fighter turns on the hero
// (GiveThingBestEnemyTarget(Me, hero)) once, the first time the hero targets it; after the flee it releases a scripting
// handle, retries StartScriptingEntity until it succeeds, walks to the nearest REGION_EXIT_POINT (the zero vector when
// there is none), re-issuing MoveToPosition whenever the last move has finished, until within 2 units, then fades
// out and dies; every early exit stops at once, and every script-thing reference count returns to its start value.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

class C3DVector { public: float X, Y, Z; };
C3DVector g_ZeroVector = { 0.0f, 0.0f, 0.0f };
struct CScriptThingImp {
    C3DVector Pos;
    long      Refs;
    virtual const C3DVector& GetPosition() const { return Pos; }
};
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
};
struct CCountedPointer {
    CScriptThingImp* Data;
};
class CScriptThing { public:
    CCountedPointer PImp;
    CScriptThing() { PImp.Data = 0; }
    explicit CScriptThing(CScriptThingImp* imp) { PImp.Data = imp; if (imp) ++imp->Refs; }
    CScriptThing(const CScriptThing& o) { PImp.Data = o.PImp.Data; if (PImp.Data) ++PImp.Data->Refs; }
    ~CScriptThing() { if (PImp.Data) --PImp.Data->Refs; }
};
static bool operator==(const CScriptThing& a, const CScriptThing& b) { return a.PImp.Data == b.PImp.Data; }

namespace NScript {
class CPredicateBase { public: virtual ~CPredicateBase() {} };
class CIsThingAlive : public CPredicateBase { public:
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& t) : Thing(t) {}
};
}

// Scenario knobs and recorders.
static int  g_frame = 0, g_terminateAt = -1, g_heroTargetFrom = -1, g_fleeAt = -1;
static int  g_giveCalls = 0, g_releaseCalls = 0, g_startCalls = 0, g_startFailures = 0, g_moveCalls = 0, g_fadeCalls = 0;
static int  g_busyPolls = 0, g_busyLeft = 0, g_distanceCalls = 0, g_farCalls = 0, g_predicateSet = 0;
static bool g_scripting = false, g_haveExit = true;
static C3DVector g_movedTo, g_checkedPos;
static CScriptThingImp g_me, g_hero, g_other, g_exit, g_scripted;

class CScriptGameResourceObjectScriptedThingBase { public:
    bool Scripting;
    CScriptGameResourceObjectScriptedThingBase() : Scripting(g_scripting) {}
    ~CScriptGameResourceObjectScriptedThingBase() {}
    bool IsScripting() const { return Scripting; }
    void Release() { ++g_releaseCalls; Scripting = false; }
    CScriptThing GetScriptThing() const { return CScriptThing(&g_scripted); }
    void MoveToPosition(const C3DVector& pos, float speed, long mode, bool a, bool b)
    {
        ++g_moveCalls; g_movedTo = pos; g_busyLeft = 2;
        CHECK(speed == 0.0f && mode == 1 && !a && b);
    }
    bool IsPerformingScriptTask() { ++g_busyPolls; if (g_busyLeft > 0) { --g_busyLeft; return true; } return false; }
};
struct GameCalls {
    void NewScriptFrame() { ++g_frame; }
    CScriptThing GetHeroTargetedThing() const { return CScriptThing(g_heroTargetFrom >= 0 && g_frame >= g_heroTargetFrom ? &g_me : &g_other); }
    CScriptThing GetHero() const { return CScriptThing(&g_hero); }
    void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const
    {
        ++g_giveCalls; CHECK(thing.PImp.Data == &g_me && target.PImp.Data == &g_hero);
    }
    bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, long priority) const
    {
        ++g_startCalls; CHECK(thing.PImp.Data == &g_me && priority == 4);
        if (g_startCalls <= g_startFailures) return false;
        handle.Scripting = true; return true;
    }
    CScriptThing GetNearestWithDefName(const CScriptThing& thing, const CCharString& name) const
    {
        CHECK(thing.PImp.Data == &g_me && strcmp(name.Text, "REGION_EXIT_POINT") == 0);
        return CScriptThing(g_haveExit ? &g_exit : 0);
    }
};
static bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance)
{
    ++g_distanceCalls; g_checkedPos = pos;
    CHECK(thing.PImp.Data == &g_scripted && distance == 2.0f);
    return g_distanceCalls <= g_farCalls;
}
static void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool remove)
{
    ++g_fadeCalls; CHECK(thing.PImp.Data == &g_me && kill && seconds == 0.5f && remove);
}
struct CQ_HangingTreeGoodScript { bool BanditsFlee; };
struct CHTG_FreedomFighter {
    void* VTable; GameCalls* Game; CScriptThing Me;
    CQ_HangingTreeGoodScript* ParentClass;
    void* PMasterData;
    bool  NowAttackingHero;
    void SetPredicate(const NScript::CPredicateBase& p)
    {
        ++g_predicateSet;
        CHECK(((const NScript::CIsThingAlive&)p).Thing.PImp.Data == &g_me);
    }
    bool IsActiveThreadTerminating()
    {
        if (g_fleeAt >= 0 && g_frame >= g_fleeAt) ParentClass->BanditsFlee = true;
        return g_terminateAt >= 0 && g_frame >= g_terminateAt;
    }
    void Main();
};

void CHTG_FreedomFighter::Main()
{
 SetPredicate(NScript::CIsThingAlive(Me));
 Game->NewScriptFrame();
 if (IsActiveThreadTerminating()) return;
 CScriptGameResourceObjectScriptedThingBase handle;
 while (!ParentClass->BanditsFlee && !NowAttackingHero) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
  if (Game->GetHeroTargetedThing() == Me) {
   if (IsActiveThreadTerminating()) return;
   Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
   NowAttackingHero = true;
  }
 }
 if (IsActiveThreadTerminating()) return;
 while (!ParentClass->BanditsFlee) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
 }
 if (IsActiveThreadTerminating()) return;
 if (handle.IsScripting())
  handle.Release();
 while (!Game->StartScriptingEntity(Me, handle, 4)) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
 }
 if (IsActiveThreadTerminating()) return;
 CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
#pragma inline_depth(0)
 CScriptThingImp* imp = (CScriptThingImp*)exitPoint.PImp.Data;
 {
 C3DVector pos = !imp ? g_ZeroVector : imp->GetPosition();
 while (IsDistanceFromThingToPositionOver(handle.GetScriptThing(), pos, 2.0f)) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
  handle.MoveToPosition(pos, 0.0f, 1, false, true);
  while (handle.IsPerformingScriptTask()) {
   Game->NewScriptFrame();
   if (IsActiveThreadTerminating()) return;
  }
  if (IsActiveThreadTerminating()) return;
 }
 }
 if (IsActiveThreadTerminating()) return;
 FadeOutAndKillEntity(Me, true, 0.5f, true);
}
#pragma inline_depth()

static void Reset(int terminateAt, int heroFrom, int fleeAt, int startFailures, int farCalls, bool haveExit, bool scripting)
{
    g_frame = 0; g_terminateAt = terminateAt; g_heroTargetFrom = heroFrom; g_fleeAt = fleeAt;
    g_giveCalls = g_releaseCalls = g_startCalls = g_moveCalls = g_fadeCalls = 0;
    g_busyPolls = g_busyLeft = g_distanceCalls = g_predicateSet = 0;
    g_startFailures = startFailures; g_farCalls = farCalls; g_haveExit = haveExit; g_scripting = scripting;
    g_hero.Refs = g_other.Refs = g_exit.Refs = g_scripted.Refs = 0;
    g_me.Refs = 1;   // the reference ff.Me holds
    g_exit.Pos.X = 10.0f; g_exit.Pos.Y = -3.0f; g_exit.Pos.Z = 4.5f;
    g_movedTo = g_ZeroVector; g_checkedPos.X = 99.0f;
}
static bool RefsBalanced() { return g_hero.Refs == 0 && g_other.Refs == 0 && g_exit.Refs == 0 && g_scripted.Refs == 0; }
int main()
{
    GameCalls game;
    CQ_HangingTreeGoodScript parent;
    CHTG_FreedomFighter ff;
    ff.Game = &game;
    ff.ParentClass = &parent;
    ff.Me.PImp.Data = &g_me;

    // 1. terminating at the first frame
    Reset(1, -1, -1, 0, 0, true, false);
    parent.BanditsFlee = false; ff.NowAttackingHero = false;
    ff.Main();
    CHECK(g_predicateSet == 1 && g_frame == 1 && g_giveCalls == 0 && g_startCalls == 0 && g_fadeCalls == 0);
    CHECK(RefsBalanced() && g_me.Refs == 1);

    // 2. full run: hero targets Me from frame 3, bandits flee at frame 6, two start failures, three moves
    Reset(-1, 3, 6, 2, 3, true, true);
    parent.BanditsFlee = false; ff.NowAttackingHero = false;
    ff.Main();
    CHECK(g_giveCalls == 1 && ff.NowAttackingHero);
    CHECK(g_releaseCalls == 1);
    CHECK(g_startCalls == 3);
    CHECK(g_moveCalls == 3 && g_movedTo.X == 10.0f && g_movedTo.Y == -3.0f && g_movedTo.Z == 4.5f);
    CHECK(g_distanceCalls == 4 && g_checkedPos.X == 10.0f && g_checkedPos.Z == 4.5f);
    CHECK(g_busyPolls == 9);
    CHECK(g_fadeCalls == 1);
    CHECK(RefsBalanced() && g_me.Refs == 1);

    // 3. no exit point: walk to the zero vector; handle not scripting, so no Release
    Reset(-1, -1, 2, 0, 1, false, false);
    parent.BanditsFlee = false; ff.NowAttackingHero = false;
    ff.Main();
    CHECK(g_giveCalls == 0 && g_releaseCalls == 0 && g_startCalls == 1);
    CHECK(g_moveCalls == 1 && g_movedTo.X == 0.0f && g_movedTo.Y == 0.0f && g_movedTo.Z == 0.0f);
    CHECK(g_fadeCalls == 1 && RefsBalanced());

    // 4. terminating during the walk: no fade, everything released
    Reset(-1, -1, 2, 0, 100, true, false);
    parent.BanditsFlee = false; ff.NowAttackingHero = false;
    g_terminateAt = 8;
    ff.Main();
    CHECK(g_moveCalls >= 1 && g_fadeCalls == 0 && RefsBalanced() && g_me.Refs == 1);

    // 5. bandits already fleeing and the hero attacked earlier: straight to scripting
    Reset(-1, 1, -1, 0, 0, true, false);
    parent.BanditsFlee = true; ff.NowAttackingHero = false;
    ff.Main();
    CHECK(g_giveCalls == 0 && g_startCalls == 1 && g_moveCalls == 0 && g_fadeCalls == 1 && RefsBalanced());

    char line[64];
    _snprintf(line, sizeof(line), "00d701c0_TEST %s", g_fail ? "FAIL" : "PASS");
    printf("%s\n", line);
    return g_fail ? 1 : 0;
}