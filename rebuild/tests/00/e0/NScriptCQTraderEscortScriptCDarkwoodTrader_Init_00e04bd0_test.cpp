// Behaviour test for NScript::CQ_TraderEscortScript::CDarkwoodTrader::Init 0x00E04BD0: the AI/brain state reset (brain
// state 1 only for an "INFECTED" trader), the infection flag, the three timers (incubation 0, follow-state comment 0,
// banter comment 60), the three flags, the green mini-map orb, persistence, no teleporter following, the narrator picked
// from the data string (INFECTED / SCARED / FRIENDLY, none otherwise) and the parent's WatchForPickpocketing thread
// spawned on a copy of Me with the name "ParentClass.WatchForPickpocketing" and an empty region.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static long g_timers[8];
class CScriptThing { public: const char* data; const char* GetDataString() const { return data; } };
class CTimer { public: int index; CTimer& operator=(long t) { g_timers[index] = t; return *this; } };
struct Game {
    void MiniMapAddMarker(const CScriptThing&, const char* graphic) { Log("Marker"); Log(graphic); }
    void SetThingPersistent(const CScriptThing&, bool p) { Log(p ? "Persistent1" : "Persistent0"); }
    void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool f) { Log(f ? "Follow1" : "Follow0"); }
    void EntitySetWillBeUsingNarrator(const CScriptThing&, const char* narrator) { Log("Narrator"); Log(narrator); }
};
struct Parent {
    char spawnName[64]; const char* spawnRegion; const char* spawnArg;
    void AddSpawnedFunction(const char* name, const char* region, CScriptThing par) { std::strcpy(spawnName, name); spawnRegion = region; spawnArg = par.data; }
};
static bool Eq(const char* a, const char* b) { return std::strcmp(a, b) == 0; }
struct CDarkwoodTrader {
    Game* Game_; CScriptThing Me; Parent* ParentClass;
    long Infected, BrainState, CurrentAIState, PreviousAIState;
    CTimer IncubationTime; bool GreetedBuddy, LeadToCamp, InSafeZone; CTimer RegulateFollowStateComment, RegulateBanterComment;
    void Init()
    {
        BrainState = 0; CurrentAIState = 0; PreviousAIState = 0;
        if (Eq(Me.GetDataString(), "INFECTED")) BrainState = 1;
        Infected = 0;
        IncubationTime = 0;
        GreetedBuddy = false; LeadToCamp = false; InSafeZone = false;
        RegulateFollowStateComment = 0;
        RegulateBanterComment = 60;
        Game_->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
        Game_->SetThingPersistent(Me, true);
        Game_->SetEntityAsFollowingHeroThroughTeleporters(Me, false);
        if (Eq(Me.GetDataString(), "INFECTED")) Game_->EntitySetWillBeUsingNarrator(Me, "INFECTED_TRADER");
        else if (Eq(Me.GetDataString(), "SCARED")) Game_->EntitySetWillBeUsingNarrator(Me, "SCARED_TRADER");
        else if (Eq(Me.GetDataString(), "FRIENDLY")) Game_->EntitySetWillBeUsingNarrator(Me, "FRIENDLY_TRADER");
        char name[64]; std::strcpy(name, "ParentClass."); std::strcat(name, "WatchForPickpocketing");
        ParentClass->AddSpawnedFunction(name, "", Me);
    }
};
static bool Run(const char* data, long brain, const char* narrator)
{
    g_log[0] = 0; std::memset(g_timers, -1, sizeof(g_timers));
    Game g; Parent p; CDarkwoodTrader t;
    t.Game_ = &g; t.Me.data = data; t.ParentClass = &p;
    t.Infected = 7; t.BrainState = 7; t.CurrentAIState = 7; t.PreviousAIState = 7;
    t.GreetedBuddy = t.LeadToCamp = t.InSafeZone = true;
    t.IncubationTime.index = 0; t.RegulateFollowStateComment.index = 1; t.RegulateBanterComment.index = 2;
    t.Init();
    char expect[256];
    std::sprintf(expect, "Marker;HUD_ORB_GREEN_SMALL;Persistent1;Follow0;%s", narrator);
    if (!Eq(g_log, expect)) { std::printf("got %s\n", g_log); return false; }
    if (t.BrainState != brain || t.Infected != 0 || t.CurrentAIState != 0 || t.PreviousAIState != 0) return false;
    if (t.GreetedBuddy || t.LeadToCamp || t.InSafeZone) return false;
    if (g_timers[0] != 0 || g_timers[1] != 0 || g_timers[2] != 60) return false;
    return Eq(p.spawnName, "ParentClass.WatchForPickpocketing") && Eq(p.spawnRegion, "") && Eq(p.spawnArg, data);
}
int main()
{
    if (!Run("INFECTED", 1, "Narrator;INFECTED_TRADER;") || !Run("SCARED", 0, "Narrator;SCARED_TRADER;")
        || !Run("FRIENDLY", 0, "Narrator;FRIENDLY_TRADER;") || !Run("", 0, "")) {
        std::printf("00e04bd0_TEST FAIL\n");
        return 1;
    }
    std::printf("00e04bd0_TEST PASS\n");
    return 0;
}