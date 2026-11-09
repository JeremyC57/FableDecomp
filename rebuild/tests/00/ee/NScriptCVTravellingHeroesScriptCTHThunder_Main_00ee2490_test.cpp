// Behaviour test for CV_TravellingHeroesScript::CTH_Thunder::Main 0x00EE2490: Thunder is kept persistent (and removed at once when
// the gameflow is past the arena), loses his information marker, becomes unpushable and sheathes; the rival handle is released if it
// was scripting, then scripting is retried every frame until it starts; he is teleported to TH_Trigger_Bowerstone unarmed and waits
// until the hero is within 25 units, faces 0.5 and spawns ParentClass.ThunderChatter; then waits for the hero to talk to him ($LINE1
// = ..._10) or hit him (not with ability 14: $LINE1 = ..._10B), plays CS_TRAVHERO_THUNDERTAUNT, marks the scene done and removes
// himself. A terminating thread returns at each check.
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

struct CHandle { bool Scripting; int Releases; bool IsValid() const { return Scripting; } void Release() { Scripting = false; ++Releases; } };
struct CParent {
    CHandle seh_Rival; bool ThunderSceneStarted; bool ThunderSceneDone; std::map<std::string, std::string> csargs;
    std::string Spawned; std::string Cutscene;
};
struct CGame {
    int Frame; bool Persistent; bool Removed; bool Info; bool Pushable; bool Sheathed; int StartAttempts; int StartSucceedsAt;
    std::string TeleportTarget; std::string Mode; int HeroNearAt; float Facing; int TalkAt; int HitAt; int HitAbility;
    void NewScriptFrame() { ++Frame; }
    bool StartScriptingEntity(CHandle& h) { ++StartAttempts; if (StartAttempts >= StartSucceedsAt) { h.Scripting = true; return true; } return false; }
};
struct CThunder {
    CGame* Game; CParent* ParentClass; long PostSavePosition; int TerminateAtFrame;
    bool IsActiveThreadTerminating() const { return TerminateAtFrame >= 0 && Game->Frame >= TerminateAtFrame; }
    bool HeroWithin25() const { return Game->Frame >= Game->HeroNearAt; }
    bool TalkedTo() const { return Game->TalkAt >= 0 && Game->Frame == Game->TalkAt; }
    bool HitBy() const { return Game->HitAt >= 0 && Game->Frame == Game->HitAt && Game->HitAbility < 0; }
    bool HitByAnySpecial() const { return Game->HitAt >= 0 && Game->Frame == Game->HitAt && Game->HitAbility >= 0; }
    bool HitBySpecial(int a) const { return HitByAnySpecial() && Game->HitAbility == a; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->Persistent = true;
        if (PostSavePosition > 900) {
            if (IsActiveThreadTerminating()) return;
            Game->Removed = true;
        }
        Game->Info = false;
        Game->Pushable = false;
        Game->Sheathed = true;
        { CParent& parent = *ParentClass; if (parent.seh_Rival.IsValid()) parent.seh_Rival.Release(); }
        while (!Game->StartScriptingEntity(ParentClass->seh_Rival)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        Game->TeleportTarget = "TH_Trigger_Bowerstone";
        Game->Mode = "UNARMED";
        while (!HeroWithin25()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->Facing = 0.5f;
        ParentClass->Spawned = std::string("ParentClass.") + std::string("ThunderChatter");
        while (!ParentClass->ThunderSceneStarted) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (TalkedTo()) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->ThunderSceneStarted = true;
                ParentClass->csargs["$LINE1"] = "TEXT_CS_B14_THUNDERTAUNT_10";
            }
            else {
                bool hitByHero = HitBy() || (HitByAnySpecial() && !HitBySpecial(14));
                if (hitByHero) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->ThunderSceneStarted = true;
                    ParentClass->csargs["$LINE1"] = "TEXT_CS_B14_THUNDERTAUNT_10B";
                }
            }
        }
        if (IsActiveThreadTerminating()) return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        ParentClass->Cutscene = "CS_TRAVHERO_THUNDERTAUNT";
        ParentClass->ThunderSceneDone = true;
        Game->Removed = true;
    }
};

static void Reset(CGame& g, CParent& p)
{
    g.Frame = 0; g.Persistent = false; g.Removed = false; g.Info = true; g.Pushable = true; g.Sheathed = false; g.StartAttempts = 0;
    g.StartSucceedsAt = 3; g.TeleportTarget = ""; g.Mode = ""; g.HeroNearAt = 8; g.Facing = 0.0f; g.TalkAt = -1; g.HitAt = -1; g.HitAbility = -1;
    p.seh_Rival.Scripting = true; p.seh_Rival.Releases = 0; p.ThunderSceneStarted = false; p.ThunderSceneDone = false; p.csargs.clear();
    p.Spawned = ""; p.Cutscene = "";
}

int main()
{
    bool ok = true;
    CGame g; CParent p; CThunder t = { &g, &p, 500, -1 };
    Reset(g, p); g.TalkAt = 12;
    t.Main();
    if (!g.Persistent || g.Info || g.Pushable || !g.Sheathed || p.seh_Rival.Releases != 1 || g.StartAttempts != 3) ok = false;
    if (g.TeleportTarget != "TH_Trigger_Bowerstone" || g.Mode != "UNARMED" || g.Facing != 0.5f) ok = false;
    if (p.Spawned != "ParentClass.ThunderChatter" || p.csargs["$LINE1"] != "TEXT_CS_B14_THUNDERTAUNT_10") ok = false;
    if (p.Cutscene != "CS_TRAVHERO_THUNDERTAUNT" || !p.ThunderSceneDone || !g.Removed) ok = false;
    Reset(g, p); g.HitAt = 11; g.HitAbility = 14; g.TalkAt = 15; t.Main(); // ability 14 does not count: the later talk starts it
    if (p.csargs["$LINE1"] != "TEXT_CS_B14_THUNDERTAUNT_10" || !p.ThunderSceneDone) ok = false;
    Reset(g, p); g.HitAt = 11; g.HitAbility = 3; t.Main();                 // ... any other special does
    if (p.csargs["$LINE1"] != "TEXT_CS_B14_THUNDERTAUNT_10B" || !p.ThunderSceneDone) ok = false;
    Reset(g, p); g.HitAt = 11; t.Main();                                   // a plain hit
    if (p.csargs["$LINE1"] != "TEXT_CS_B14_THUNDERTAUNT_10B") ok = false;
    Reset(g, p); p.seh_Rival.Scripting = false; g.TalkAt = 9; t.PostSavePosition = 950; t.Main();
    if (p.seh_Rival.Releases != 0 || !g.Removed || p.csargs["$LINE1"] != "TEXT_CS_B14_THUNDERTAUNT_10") ok = false;
    t.PostSavePosition = 500;
    Reset(g, p); g.TalkAt = 12; t.TerminateAtFrame = 5; t.Main();          // terminates while waiting for the hero
    if (p.Spawned != "" || p.ThunderSceneDone || g.Removed) ok = false;
    if (!ok) { std::printf("00ee2490_TEST FAIL\n"); return 1; }
    std::printf("00ee2490_TEST PASS\n");
    return 0;
}