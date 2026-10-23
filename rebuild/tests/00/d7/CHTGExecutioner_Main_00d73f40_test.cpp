// 00d73f40 behaviour test: CHTG_Executioner::Main (match.cpp body verbatim, pragmas dropped; the main loop is
// for(;;) with the terminate check at its top), host stand-ins. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const std::string& s) { g_log += s; g_log += ";"; }
enum EHeroAbility { HERO_ABILITY_NONE };
struct CCharString {
    std::string s;
    CCharString(const char* text, long length = -1) : s(text) { (void)length; }
};
static int g_hitHero = 0, g_hitAbility = 0, g_hitAbility14 = 0, g_hitAny = 0;   // hit messages pending on Me
struct CScriptThing {
    std::string Name;
    CScriptThing() {}
    CScriptThing(const char* n) : Name(n) {}
    bool MsgIsHitBy(const CCharString& name) const
    {
        if (name.s == "SCRIPT_NAME_HERO") { if (g_hitHero > 0) { --g_hitHero; return true; } return false; }
        if (g_hitAny > 0) { --g_hitAny; return true; }
        return false;
    }
    bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const
    { (void)name; if ((int)ability == 14 && g_hitAbility14 > 0) { --g_hitAbility14; return true; } return false; }
    bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const
    { (void)name; if (g_hitAbility > 0) { --g_hitAbility; return true; } return false; }
};
struct CScriptThingVector {
    std::vector<CScriptThing> v;
    unsigned int size() const { return (unsigned int)v.size(); }
    CScriptThing& operator[](unsigned int i) { return v[i]; }
    void clear() { v.clear(); }
};
struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
};
static int g_releases = 0;
static bool g_scripting = true;
struct CScriptGameResourceObjectScriptedThingBase {
    bool IsScripting() { return g_scripting; }
    void Release() { ++g_releases; }
};
struct CQ_HangingTreeGoodScript {
    bool ExecutionerBeenAttacked;
    bool ExecutionerBeenAttackedByHero;
    long HTG_State;
    CScriptThingVector CurrentGuardVector;
};
static CQ_HangingTreeGoodScript* g_parent = 0;
struct CGameScriptInterface {
    CScriptThing Hero;
    long Frames, FrameLimit;
    long StateAtFrame;        // HTG_State becomes 7 at this frame
    long ClearFlagsAtFrame;   // attacked flags are cleared (by the quest) at this frame
    CGameScriptInterface() : Hero("hero"), Frames(0), FrameLimit(100), StateAtFrame(0), ClearFlagsAtFrame(0) {}
    void NewScriptFrame() const
    {
        CGameScriptInterface* g = const_cast<CGameScriptInterface*>(this);
        ++g->Frames;
        if (Frames >= StateAtFrame) g_parent->HTG_State = 7;
        if (Frames >= ClearFlagsAtFrame) { g_parent->ExecutionerBeenAttacked = false; g_parent->ExecutionerBeenAttackedByHero = false; }
    }
    CScriptThing& GetHero() { return Hero; }
    long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const
    {
        logs("getall-" + name.s);
        things.v.push_back(CScriptThing("aud1")); things.v.push_back(CScriptThing("aud2"));
        return 2;
    }
    void EntitySetThingAsAllyOfThing(const CScriptThing& a0, const CScriptThing& a1) const { logs("ally-" + a0.Name + "-" + a1.Name); }
    void SetIsThingForcePushable(const CScriptThing& a0, bool a1) const { logs("push-" + a0.Name + (a1 ? "-t" : "-f")); }
};
static int g_filters = 0;
struct CEntityScriptBase {
    CGameScriptInterface* Game;
    CScriptThing          Me;
    long Checks, TerminateAtCheck;
    void RegisterFilter(CIsThingAlive& filter) { ++g_filters; (void)filter; }
    bool IsActiveThreadTerminating() const
    {
        CEntityScriptBase* s = const_cast<CEntityScriptBase*>(this);
        ++s->Checks;
        if (TerminateAtCheck >= 0 && Checks >= TerminateAtCheck) return true;
        return Game->Frames >= Game->FrameLimit;
    }
};
struct CHTG_Executioner : CEntityScriptBase {
    CQ_HangingTreeGoodScript* ParentClass;
    CScriptThingVector AudienceMembers;
    void Main();
};

void CHTG_Executioner::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase executioner;
    AudienceMembers.clear();
    Game->GetAllThingsWithScriptName("HTG_Audience", AudienceMembers);
    Game->SetIsThingForcePushable(Me, false);
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
    for (unsigned int i = 0; i < AudienceMembers.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetThingAsAllyOfThing(Me, AudienceMembers[i]);
        Game->EntitySetThingAsAllyOfThing(AudienceMembers[i], Me);
    }
    if (IsActiveThreadTerminating())
        return;
    for (unsigned int j = 0; j < ParentClass->CurrentGuardVector.size(); ++j) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetThingAsAllyOfThing(Me, ParentClass->CurrentGuardVector[j]);
        Game->EntitySetThingAsAllyOfThing(ParentClass->CurrentGuardVector[j], Me);
    }
    if (IsActiveThreadTerminating())
        return;
    while (ParentClass->HTG_State != 7) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        {
            return;
        }
    if (executioner.IsScripting())
        executioner.Release();
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                {
                    return;
                }
            ParentClass->ExecutionerBeenAttackedByHero = true;
            while (ParentClass->ExecutionerBeenAttackedByHero) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
                        return;
                    }
            }
            if (IsActiveThreadTerminating())
                {
                    return;
                }
        } else if (Me.MsgIsHitBy("")) {
            if (IsActiveThreadTerminating())
                {
                    return;
                }
            ParentClass->ExecutionerBeenAttacked = true;
            while (ParentClass->ExecutionerBeenAttacked) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
                        return;
                    }
            }
            if (IsActiveThreadTerminating())
                {
                    return;
                }
        }
        Game->NewScriptFrame();
        continue;
    }
}

static void Setup(CGameScriptInterface& g, CQ_HangingTreeGoodScript& p, CHTG_Executioner& e)
{
    p.ExecutionerBeenAttacked = false; p.ExecutionerBeenAttackedByHero = false; p.HTG_State = 0;
    p.CurrentGuardVector.v.clear(); p.CurrentGuardVector.v.push_back(CScriptThing("guard"));
    g_parent = &p;
    e.Game = &g; e.Me = CScriptThing("exec"); e.Checks = 0; e.TerminateAtCheck = -1; e.ParentClass = &p;
    e.AudienceMembers.v.clear(); e.AudienceMembers.v.push_back(CScriptThing("stale"));
    g_log = ""; g_hitHero = g_hitAbility = g_hitAbility14 = g_hitAny = 0; g_releases = 0; g_scripting = true; g_filters = 0;
}

int main()
{
    // 1: terminating at once after the first frame
    {
        CGameScriptInterface g; CQ_HangingTreeGoodScript p; CHTG_Executioner e; Setup(g, p, e);
        g.FrameLimit = 1;
        e.Main();
        CHECK(g_filters == 1 && g_log == "" && e.AudienceMembers.size() == 1);
    }
    // 2: allies set up, wait for state 7 (frame 3), release the scripting handle, hero hit -> flag until cleared at frame 6
    {
        CGameScriptInterface g; CQ_HangingTreeGoodScript p; CHTG_Executioner e; Setup(g, p, e);
        g.StateAtFrame = 3; g.ClearFlagsAtFrame = 100; g.FrameLimit = 9;
        g_hitHero = 1;
        e.Main();
        CHECK(e.AudienceMembers.size() == 2);
        CHECK(g_log == "getall-HTG_Audience;push-exec-f;ally-exec-hero;ally-hero-exec;ally-exec-aud1;ally-aud1-exec;"
                       "ally-exec-aud2;ally-aud2-exec;ally-exec-guard;ally-guard-exec;");
        CHECK(p.HTG_State == 7 && g_releases == 1);
        CHECK(p.ExecutionerBeenAttackedByHero);         // still waiting when the thread terminated
        CHECK(g.Frames == 9);
    }
    // 3: hit by a special ability (not ability 14) counts as a hero hit; flag cleared at frame 5, then idle frames
    {
        CGameScriptInterface g; CQ_HangingTreeGoodScript p; CHTG_Executioner e; Setup(g, p, e);
        g.StateAtFrame = 2; g.ClearFlagsAtFrame = 5; g.FrameLimit = 8;
        g_scripting = false; g_hitAbility = 1;
        e.Main();
        CHECK(g_releases == 0);
        CHECK(!p.ExecutionerBeenAttackedByHero && !p.ExecutionerBeenAttacked);
        CHECK(g_hitAbility == 0);
        CHECK(g.Frames == 8);
    }
    // 4: ability 14 is excluded from hero hits; a plain hit by anyone sets ExecutionerBeenAttacked
    {
        CGameScriptInterface g; CQ_HangingTreeGoodScript p; CHTG_Executioner e; Setup(g, p, e);
        g.StateAtFrame = 1; g.ClearFlagsAtFrame = 100; g.FrameLimit = 6;
        g_hitAbility = 1; g_hitAbility14 = 1; g_hitAny = 1;
        e.Main();
        CHECK(!p.ExecutionerBeenAttackedByHero);
        CHECK(p.ExecutionerBeenAttacked);
    }
    // 5: terminate inside the audience loop (check 1 = after the first frame, 2 = audience[0], 3 = audience[1])
    {
        CGameScriptInterface g; CQ_HangingTreeGoodScript p; CHTG_Executioner e; Setup(g, p, e);
        e.TerminateAtCheck = 3;
        e.Main();
        CHECK(g_log == "getall-HTG_Audience;push-exec-f;ally-exec-hero;ally-hero-exec;ally-exec-aud1;ally-aud1-exec;");
    }
    if (!g_fail) printf("00d73f40_TEST PASS\n");
    return g_fail;
}