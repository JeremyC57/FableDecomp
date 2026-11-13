// Behaviour test for 0x00E621F0 NScript::CV_BodyGuardScript::CBodyGuard::SetHenchmanAsHired(bool): a self-contained host model of
// the row's logic (hire / dismiss, health-bar icon per henchman type, wage bill through GetGoldCost, the dismiss-time warp-out when
// the home level is not loaded, the terminate checks) over a recording game-interface mock.
#include <cstdio>
#include <cstring>

static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct Thing { int id; };
struct Vec { float X, Y, Z; };
struct Colour { unsigned char B, G, R, A; };

struct Game {
    bool levelLoaded;
    int terminateAfter;       // IsActiveThreadTerminating returns true from this call count on (-1: never)
    int terminateCalls;
    int scriptingFailures;    // StartScriptingEntity fails this many times first
    Vec pos;
    Vec effectPos;
    Colour lastColour;
    char lastIcon[64];
    long nextBarId;
    void ClearThingBestEnemyTarget(const Thing&) { Log("ClearTarget"); }
    const Thing& GetHero() { static Thing hero = { 99 }; return hero; }
    void SetEntityAsRegionFollowing(const Thing& leader, const Thing& who, bool on) { Log(on && leader.id == 99 && who.id == 7 ? "RegionFollow1" : (!on && leader.id == 99 ? "RegionFollow0" : "RegionFollowBad")); }
    void EntityFollowThing(const Thing& who, const Thing& what, float dist, bool b) { Log(who.id == 7 && what.id == 99 && dist == 1.0f && b ? "Follow" : "FollowBad"); }
    void EntitySetThingAsAllyOfThing(const Thing&, const Thing& b) { Log(b.id == 99 ? "Ally" : "AllyBad"); }
    void EntitySetInFaction(const Thing&, const char* f) { Log(f); }
    void DisplayQuestInfo(bool b) { Log(b ? "QuestInfo1" : "QuestInfo0"); }
    long AddQuestInfoBarHealth(const Thing&, const Colour& c, const char* icon, float scale) {
        lastColour = c; std::strcpy(lastIcon, icon); Log(scale == 1.0f ? "AddBar" : "AddBarBad"); return nextBarId;
    }
    void EntitySetAsAllowedToFollowHero(const Thing&, bool b) { Log(b ? "AllowFollow1" : "AllowFollow0"); }
    void RemoveQuestInfoElement(long id) { Log(id == 42 ? "RemoveBar42" : "RemoveBarBad"); }
    void EntityStopFollowing(const Thing&) { Log("StopFollow"); }
    bool IsLevelLoaded(const char* name) { Log(std::strcmp(name, "HomeMap") == 0 ? "IsLevelLoaded" : "IsLevelLoadedBad"); return levelLoaded; }
    void EntitySetAsDamageable(const Thing&, bool b) { Log(b ? "Damageable1" : "Damageable0"); }
    void EntitySetTargetable(const Thing&, bool b) { Log(b ? "Targetable1" : "Targetable0"); }
    void SetIsPushableByHero(Thing t, bool b) { Log(t.id == 7 && !b ? "Pushable0" : "PushableBad"); }
    void SetIsThingForcePushable(const Thing&, bool b) { Log(b ? "ForcePush1" : "ForcePush0"); }
    bool StartScriptingEntity(const Thing&, int priority) { Log(priority == 4 ? "StartScripting" : "StartScriptingBad"); return scriptingFailures-- <= 0; }
    void NewScriptFrame() { Log("NSF"); }
    void CreateEffect(const char* def, const Vec& p, const char* name, float angle, bool a, bool b) {
        effectPos = p; Log(std::strcmp(def, "HENCHMAN_WARP") == 0 && std::strcmp(name, "HenchmanWarp") == 0 && angle == 0.0f && !a && !b ? "Effect" : "EffectBad");
    }
};

struct Parent {
    long TotalWages;
    Thing HenchmanList[8];
    int count;
    long HenchmanGlobalStatus[4];
    long HenchmanDeadRespawnTimer[4];
};

struct ScriptDef { long HenchmanCheapGoldCost, HenchmanExpensiveGoldCost; };
static ScriptDef g_def = { 10, 25 };
static int g_fadeCalls;

struct BodyGuard {
    Game* game;
    Thing Me;
    Parent* ParentClass;
    long HenchManType;
    long BrainState;
    long MyHealthBarID;

    bool IsActiveThreadTerminating() { int n = game->terminateCalls++; return game->terminateAfter >= 0 && n >= game->terminateAfter; }
    long GetGoldCost() {
        switch (HenchManType) {
        case 0: case 2: return g_def.HenchmanCheapGoldCost;
        case 1: case 3: return g_def.HenchmanExpensiveGoldCost;
        }
        return 0;
    }
    void Bar(const char* icon) { Colour c; c.R = 0; c.G = 255; c.B = 0; c.A = 255; MyHealthBarID = game->AddQuestInfoBarHealth(Me, c, icon, 1.0f); }
    void SetHenchmanAsHired(bool hired) {
        if (hired) {
            if (IsActiveThreadTerminating()) return;
            BrainState = 2;
            game->ClearThingBestEnemyTarget(Me);
            game->SetEntityAsRegionFollowing(game->GetHero(), Me, true);
            game->EntityFollowThing(Me, game->GetHero(), 1.0f, true);
            game->EntitySetThingAsAllyOfThing(Me, game->GetHero());
            game->EntitySetInFaction(Me, "FACTION_HERO");
            ParentClass->HenchmanList[ParentClass->count++] = Me;
            ParentClass->TotalWages += GetGoldCost();
            game->DisplayQuestInfo(true);
            switch (HenchManType) {
            case 2: Bar("HUD_QUEST_ICON_BODYGUARD_GOOD_01"); break;
            case 3: Bar("HUD_QUEST_ICON_BODYGUARD_GOOD_02"); break;
            case 0: Bar("HUD_QUEST_ICON_BODYGUARD_BAD_01"); break;
            case 1: Bar("HUD_QUEST_ICON_BODYGUARD_BAD_02"); break;
            }
        } else {
            if (IsActiveThreadTerminating()) return;
            game->EntitySetAsAllowedToFollowHero(Me, false);
            game->RemoveQuestInfoElement(MyHealthBarID);
            BrainState = 1;
            game->ClearThingBestEnemyTarget(Me);
            game->SetEntityAsRegionFollowing(game->GetHero(), Me, false);
            game->EntityStopFollowing(Me);
            game->EntitySetInFaction(Me, "FACTION_BODYGUARD");
            for (int i = 0; i < ParentClass->count; ++i) {
                if (ParentClass->HenchmanList[i].id == Me.id) {
                    for (int k = i; k + 1 < ParentClass->count; ++k) ParentClass->HenchmanList[k] = ParentClass->HenchmanList[k + 1];
                    --ParentClass->count;
                    break;
                }
            }
            ParentClass->TotalWages -= GetGoldCost();
            if (game->IsLevelLoaded("HomeMap")) {
                if (IsActiveThreadTerminating()) return;
            } else {
                if (IsActiveThreadTerminating()) return;
                game->EntitySetAsDamageable(Me, false);
                game->EntitySetTargetable(Me, false);
                game->SetIsPushableByHero(Me, false);
                game->SetIsThingForcePushable(Me, false);
                while (!game->StartScriptingEntity(Me, 4)) {
                    game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                Vec p = game->pos; p.Z += 1.0f;
                game->CreateEffect("HENCHMAN_WARP", p, "HenchmanWarp", 0.0f, false, false);
                ParentClass->HenchmanGlobalStatus[HenchManType] = 1;
                ParentClass->HenchmanDeadRespawnTimer[HenchManType] = 0;
                ++g_fadeCalls;
                Log("Fade");
            }
        }
    }
};

static int g_fail;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("check failed: %s\n", what); g_fail = 1; } }

static void Setup(Game& g, Parent& p, BodyGuard& b, long type) {
    std::memset(&g, 0, sizeof(g)); g.terminateAfter = -1; g.nextBarId = 42; g.pos.X = 1.0f; g.pos.Y = 2.0f; g.pos.Z = 3.0f;
    std::memset(&p, 0, sizeof(p)); p.TotalWages = 100; if (type < 4) p.HenchmanDeadRespawnTimer[type] = 240;
    b.game = &g; b.Me.id = 7; b.ParentClass = &p; b.HenchManType = type; b.BrainState = 1; b.MyHealthBarID = -1;
    g_log[0] = 0; g_fadeCalls = 0;
}

int main()
{
    Game g; Parent p; BodyGuard b;
    // Hire every type: icon by type (2/3 good, 0/1 bad), cheap (0/2) or expensive (1/3) wage.
    const char* icons[4] = { "HUD_QUEST_ICON_BODYGUARD_BAD_01", "HUD_QUEST_ICON_BODYGUARD_BAD_02", "HUD_QUEST_ICON_BODYGUARD_GOOD_01", "HUD_QUEST_ICON_BODYGUARD_GOOD_02" };
    const long wage[4] = { 10, 25, 10, 25 };
    for (long t = 0; t < 4; ++t) {
        Setup(g, p, b, t);
        b.SetHenchmanAsHired(true);
        Check(b.BrainState == 2 && p.count == 1 && p.HenchmanList[0].id == 7, "hire state");
        Check(p.TotalWages == 100 + wage[t], "hire wage");
        Check(b.MyHealthBarID == 42 && std::strcmp(g.lastIcon, icons[t]) == 0, "hire icon");
        Check(g.lastColour.R == 0 && g.lastColour.G == 255 && g.lastColour.B == 0 && g.lastColour.A == 255, "bar colour");
        Check(std::strcmp(g_log, "ClearTarget;RegionFollow1;Follow;Ally;FACTION_HERO;QuestInfo1;AddBar;") == 0, "hire log");
    }
    // Unknown type: no bar, no wage.
    Setup(g, p, b, 4);
    b.SetHenchmanAsHired(true);
    Check(b.MyHealthBarID == -1 && p.TotalWages == 100, "hire type 4");
    // Hire then dismiss on a loaded home level: list and wage restored, no warp.
    Setup(g, p, b, 1);
    p.HenchmanList[0].id = 3; p.count = 1;
    b.SetHenchmanAsHired(true);
    g.levelLoaded = true; g_log[0] = 0;
    b.SetHenchmanAsHired(false);
    Check(b.BrainState == 1 && p.count == 1 && p.HenchmanList[0].id == 3 && p.TotalWages == 100, "dismiss loaded");
    Check(std::strcmp(g_log, "AllowFollow0;RemoveBar42;ClearTarget;RegionFollow0;StopFollow;FACTION_BODYGUARD;IsLevelLoaded;") == 0, "dismiss loaded log");
    Check(p.HenchmanDeadRespawnTimer[1] == 240 && g_fadeCalls == 0, "no warp when loaded");
    // Dismiss with the home level unloaded: scripting retried once, warp effect 1 above the henchman, status dead, timer 0, fade.
    Setup(g, p, b, 3);
    b.MyHealthBarID = 42; p.HenchmanList[0].id = 7; p.count = 1; p.TotalWages = 125; g.scriptingFailures = 1;
    b.SetHenchmanAsHired(false);
    Check(p.count == 0 && p.TotalWages == 100, "dismiss unloaded list/wage");
    Check(g.effectPos.X == 1.0f && g.effectPos.Y == 2.0f && g.effectPos.Z == 4.0f, "warp position");
    Check(p.HenchmanGlobalStatus[3] == 1 && p.HenchmanDeadRespawnTimer[3] == 0 && g_fadeCalls == 1, "dead status / timer / fade");
    Check(std::strcmp(g_log, "AllowFollow0;RemoveBar42;ClearTarget;RegionFollow0;StopFollow;FACTION_BODYGUARD;IsLevelLoaded;Damageable0;Targetable0;Pushable0;ForcePush0;StartScripting;NSF;StartScripting;Effect;Fade;") == 0, "dismiss unloaded log");
    // Terminating thread: nothing happens on either path; terminate inside the scripting wait stops before the warp.
    Setup(g, p, b, 0); g.terminateAfter = 0;
    b.SetHenchmanAsHired(true);
    Check(g_log[0] == 0 && b.BrainState == 1, "terminate hire");
    b.SetHenchmanAsHired(false);
    Check(g_log[0] == 0, "terminate dismiss");
    Setup(g, p, b, 0); g.scriptingFailures = 5; g.terminateAfter = 3;
    b.SetHenchmanAsHired(false);
    Check(g_fadeCalls == 0 && p.HenchmanGlobalStatus[0] == 0, "terminate in scripting wait");
    if (g_fail) { std::printf("00e621f0_TEST FAIL\n"); return 1; }
    std::printf("00e621f0_TEST PASS\n");
    return 0;
}