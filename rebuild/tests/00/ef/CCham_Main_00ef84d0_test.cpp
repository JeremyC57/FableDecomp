// Behaviour test for 0x00EF84D0 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks the Leader lookup, the And(IsThing(Me), IsThing(leader)) predicate, the early exit after the opening frame,
// the scripted-control retry loop (priority 4, one frame per failed attempt), MoveAlong(leader, 1.0, true) and the movement wait,
// and that the leader and the predicate copies are released on every exit.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0;                  // live CScriptThing copies (every exit must release the watched thing)
static int g_iatt = 0;                  // IsActiveThreadTerminating call count
static int g_termAt = 1 << 30;          // ... it returns true from this call number on
static int g_termFrame = 1 << 30;       // ... or once this many script frames have run
static int g_frames = 0;                // NewScriptFrame count
static bool g_badName = false;          // a callee saw an unexpected name / argument
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_termFrame; }
static void Reset() { g_live = 0; g_iatt = 0; g_termAt = 1 << 30; g_termFrame = 1 << 30; g_frames = 0; g_badName = false; }

struct CCharString {
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
static int g_startFailures = 0, g_movingPolls = 0, g_moveAlongs = 0, g_releases = 0, g_scriptedLive = 0;
struct CScriptGameResourceObjectScriptedThingBase {
    bool Scripting;
    CScriptGameResourceObjectScriptedThingBase() : Scripting(false) { ++g_scriptedLive; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_scriptedLive; }
    bool IsScripting() { return Scripting; }
    void Release() { ++g_releases; }
    void MoveAlong(const CScriptThing& thing, float distance, bool flag) { if (thing.Id != 5 || distance != 1.0f || !flag) g_badName = true; ++g_moveAlongs; }
    bool IsMoving() { return g_movingPolls-- > 0; }
};
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing GetThingWithScriptName(const CCharString& name) const { if (strcmp(name.Text, "Leader") != 0) g_badName = true; return CScriptThing(5); }
    bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority)
    {
        if (thing.Id != 1 || (int)priority != 4) g_badName = true;
        if (g_startFailures > 0) { --g_startFailures; return false; }
        object.Scripting = true;
        return true;
    }
};
struct CFilterBase { };
struct CIsThing {
    CScriptThing m_thing;
    CIsThing(const CScriptThing& thing) : m_thing(thing) { }
};
struct CAnd : CFilterBase {
    CIsThing m_a;
    CIsThing m_b;
    CAnd(const CIsThing& a, const CIsThing& b) : m_a(a), m_b(b) { }
    ~CAnd() { }
};
inline CAnd And(const CIsThing& a, const CIsThing& b) { return CAnd(a, b); }
static int g_predicateOk = 0;
struct CCham {
    CGameScriptInterface* Game;
    CScriptThing          Me;
    CCham() : Me(1) { }
    void SetPredicate(const CFilterBase& predicate) { const CAnd& f = (const CAnd&)predicate; if (f.m_a.m_thing.Id == 1 && f.m_b.m_thing.Id == 5) ++g_predicateOk; }
    bool IsActiveThreadTerminating() const { return Term(); }
    void Main();
};

void CCham::Main()
{
    CScriptThing leader = Game->GetThingWithScriptName("Leader");
    SetPredicate(And(CIsThing(Me), CIsThing(leader)));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
#pragma inline_depth(0)
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    scripted.MoveAlong(leader, 1.0f, true);
    while (scripted.IsMoving()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
#pragma inline_depth()

static void Run(int termAt, int startFailures, int movingPolls)
{
    Reset(); g_termAt = termAt; g_startFailures = startFailures; g_movingPolls = movingPolls;
    g_moveAlongs = 0; g_releases = 0; g_scriptedLive = 0; g_predicateOk = 0;
    static CGameScriptInterface game;
    {
        CCham c; c.Game = &game;
        g_live = 0;
        c.Main();
        CHECK(g_live == 0 && g_scriptedLive == 0);
    }
}
int main()
{
    Run(1, 0, 0); CHECK(g_predicateOk == 1 && g_frames == 1 && g_moveAlongs == 0);
    Run(1 << 30, 2, 3); CHECK(g_predicateOk == 1 && g_moveAlongs == 1 && g_frames == 1 + 2 + 3 && g_releases == 0 && !g_badName);
    // opening(1); two failed starts each followed by a check (2, 3) -> terminates at the second
    Run(3, 5, 0); CHECK(g_moveAlongs == 0 && g_frames == 3);
    // opening(1), post-start(2) terminates before MoveAlong
    Run(2, 0, 3); CHECK(g_moveAlongs == 0 && g_frames == 1);
    // opening(1), post-start(2), movement poll check(3) terminates
    Run(3, 0, 4); CHECK(g_moveAlongs == 1 && g_frames == 2);
    char line[64];
    _snprintf(line, sizeof(line), "00ef84d0_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}