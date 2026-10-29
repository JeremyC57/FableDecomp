// Behaviour test for 0x00E5DE80 NScript::CV_BodyGuardScript::CBodyGuard::Init: the henchman's state is reset (no health bar,
// brain state 1, health states 0, not freaked out), the interface calls run in retail's order with retail's arguments, the
// wander centre is the thing's home position and the anger timer is set to 0.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:
    int Id;
    C3DVector GetHomePos() const { C3DVector v; v.X = 1.0f; v.Y = 2.0f; v.Z = 3.0f; return v; }
};
class CGameScriptInterface { public:
    char b[64];
    void SetTimer(long timer, long time) { std::sprintf(b, "timer(%ld,%ld)", timer, time); Log(b); }
    void EntitySetAsAllowedToFollowHero(const CScriptThing& t, bool a) const { char x[64]; std::sprintf(x, "follow(%d,%d)", t.Id, a); Log(x); }
    void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing& t, bool a) const { char x[64]; std::sprintf(x, "respond(%d,%d)", t.Id, a); Log(x); }
    void SetThingHasInformation(const CScriptThing& t, bool a, bool b2, bool c) const { char x[64]; std::sprintf(x, "info(%d,%d,%d,%d)", t.Id, a, b2, c); Log(x); }
    void SetThingPersistent(const CScriptThing& t, bool a) const { char x[64]; std::sprintf(x, "persist(%d,%d)", t.Id, a); Log(x); }
    void EntitySetThingAsWantingMoney(const CScriptThing& t, long m) const { char x[64]; std::sprintf(x, "money(%d,%ld)", t.Id, m); Log(x); }
    void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing& t, bool a) const { char x[64]; std::sprintf(x, "teleport(%d,%d)", t.Id, a); Log(x); }
    void EntitySheatheWeapons(const CScriptThing& t, bool a) const { char x[64]; std::sprintf(x, "sheathe(%d,%d)", t.Id, a); Log(x); }
    void EntitySetInFaction(const CScriptThing& t, const CCharString& f) const { char x[64]; std::sprintf(x, "faction(%d,%s)", t.Id, f.Text); Log(x); }
    void SetWanderCentrePoint(CScriptThing t, C3DVector c) { char x[64]; std::sprintf(x, "centre(%d,%g,%g,%g)", t.Id, c.X, c.Y, c.Z); Log(x); }
    void SetWanderMinDistance(CScriptThing t, float d) { char x[64]; std::sprintf(x, "min(%d,%g)", t.Id, d); Log(x); }
    void SetWanderMaxDistance(CScriptThing t, float d) { char x[64]; std::sprintf(x, "max(%d,%g)", t.Id, d); Log(x); }
};
static CGameScriptInterface* g_pGameScriptInterface;
class CTimer { public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
struct CBodyGuard {
    CGameScriptInterface* Game; CScriptThing Me;
    long HenchManType, BrainState, CurrentHealthState, PreviousHealthState; bool FreakedOut;
    CTimer AngerTimer, AttackedBySummonersTimer; long MyHealthBarID;
    void Init()
    {
        MyHealthBarID = -1;
        BrainState = 1;
        CurrentHealthState = 0;
        PreviousHealthState = 0;
        FreakedOut = false;
        Game->EntitySheatheWeapons(Me, false);
        Game->EntitySetInFaction(Me, "FACTION_BODYGUARD");
        Game->SetEntityAsFollowingHeroThroughTeleporters(Me, true);
        Game->SetThingHasInformation(Me, false, true, false);
        Game->SetThingPersistent(Me, true);
        Game->EntitySetAsAllowedToFollowHero(Me, false);
        Game->EntitySetThingAsWantingMoney(Me, 0);
        Game->EntitySetAsRespondingToFollowAndWaitExpressions(Me, false);
        Game->SetWanderMinDistance(Me, 0.0f);
        Game->SetWanderMaxDistance(Me, 10.0f);
        Game->SetWanderCentrePoint(Me, Me.GetHomePos());
        AngerTimer = 0;
    }
};

int main()
{
    CGameScriptInterface game; g_pGameScriptInterface = &game;
    CBodyGuard g; g.Game = &game; g.Me.Id = 7; g.HenchManType = 3; g.BrainState = 9; g.CurrentHealthState = 9;
    g.PreviousHealthState = 9; g.FreakedOut = true; g.AngerTimer.TimerIndex = 42; g.AttackedBySummonersTimer.TimerIndex = 43;
    g.MyHealthBarID = 5;
    g.Init();
    int ok = g.MyHealthBarID == -1 && g.BrainState == 1 && g.CurrentHealthState == 0 && g.PreviousHealthState == 0
             && !g.FreakedOut && g.HenchManType == 3
             && !std::strcmp(g_log, "sheathe(7,0);faction(7,FACTION_BODYGUARD);teleport(7,1);info(7,0,1,0);persist(7,1);"
                                    "follow(7,0);money(7,0);respond(7,0);min(7,0);max(7,10);centre(7,1,2,3);timer(42,0);");
    if (!ok) { std::printf("00e5de80_TEST FAIL\n"); return 1; }
    std::printf("00e5de80_TEST PASS\n");
    return 0;
}