// Behaviour test for NScript::CV_FishermanScript::NPCAttitudeProcess(CScriptThing) (0x00E7C010): wait until the hero hits the
// fisherman (or heals him); if his health is above 1 the quest relation counter 2 is set to 2, FishermanMustRunAway is raised,
// the fisherman is scripted, a widescreen movie plays (teleports to the NPC/hero stands and camera point when within 10 m,
// the HATE line if he is alive), he walks to MK_FI_HERO, the screen fades out, he is removed and the camera and screen restored.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static long RelationCounters[3];
struct CScriptThing { int id; bool alive; float dist; bool IsAlive() const { return alive; } float GetAngleXY() const { return 1.5f; } };
struct CGame {
    int frames; int hitAt; int healAt; float health; int scriptedAt; int speakFrames; bool nearStands; bool cameraAlive;
    void NewScriptFrame() { ++frames; if (speakFrames > 0) --speakFrames; }
    bool MsgIsHitBy(const char*) { return frames == hitAt; }
    bool MsgIsHitByAnySpecialAbilityFrom(const char*) { return frames == healAt; }
    bool MsgIsHitBySpecialAbilityFrom(int ability, const char*) { return ability == 8 && frames == healAt; }
    float GetHealth(CScriptThing) { return health; }
    bool StartScriptingEntity(CScriptThing, int, int) { return frames >= scriptedAt; }
    void StartMovieSequence(const char*) { Log("movie"); }
    void SheatheHeroWeapons() { Log("sheathe"); }
    CScriptThing GetNearestWithScriptName(CScriptThing, const char* n) { CScriptThing t = { 5, true, nearStands ? 2.0f : 50.0f }; if (std::strcmp(n, "NPC_Camera") == 0) t.alive = cameraAlive; return t; }
    CScriptThing GetHero() { CScriptThing t = { 1, true, nearStands ? 2.0f : 50.0f }; return t; }
    void EntityTeleportToThing(CScriptThing, CScriptThing, bool) { Log("teleport"); }
    void EntitySetFacingAngle(CScriptThing, float a, bool) { Log(a == 1.5f ? "face" : "badface"); }
    void CameraUseCameraPoint(CScriptThing, int, int, float, int, int) { Log("camera"); }
    void EntitySetFacingAngleTowardsThing(CScriptThing, CScriptThing, bool) { Log("facehero"); }
    void Speak(const char* t) { Log("speak"); Log(t); speakFrames = 2; }
    bool IsPerformingScriptTask() { return speakFrames > 0; }
    void MoveToThing(const char* n, float d) { Log("move"); Log(n); if (d != 3.0f) Log("baddist"); }
    void Pause(float s) { Log(s == 1.5f ? "pause1.5" : s == 2.0f ? "pause2" : "pause?"); }
    void FadeScreenOut(float, float, int) { Log("fadeout"); }
    void RemoveThing(CScriptThing, bool, bool) { Log("remove"); }
    void CameraDefault() { Log("cameradefault"); }
    void FadeScreenIn() { Log("fadein"); }
};
struct CV_FishermanScript {
    CGame* Game; bool FishermanMustRunAway; int termAfter, termCalls;
    bool IsActiveThreadTerminating() { return ++termCalls > termAfter; }
    bool IsDistanceBetweenThingsUnder(CScriptThing, CScriptThing b, float d) { return b.dist < d; }
    void NPCAttitudeProcess(CScriptThing npc)
    {
        Log("predicate");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        for (;;) {
            if (Game->MsgIsHitBy("SCRIPT_NAME_HERO") ||
                (Game->MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && Game->MsgIsHitBySpecialAbilityFrom(8, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return;
                if (!(Game->GetHealth(npc) <= 1.0f)) break;
            }
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        RelationCounters[2] = 2;
        FishermanMustRunAway = true;
        while (!Game->StartScriptingEntity(npc, 0, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        {
            Game->StartMovieSequence("");
            Log("pause-on");
            Game->SheatheHeroWeapons();
            CScriptThing npcStand = Game->GetNearestWithScriptName(npc, "NPC_NPCStand");
            if (IsDistanceBetweenThingsUnder(npc, npcStand, 10.0f)) {
                if (IsActiveThreadTerminating()) return;
                Game->EntityTeleportToThing(npc, npcStand, false);
                Game->EntitySetFacingAngle(npc, npcStand.GetAngleXY(), true);
                CScriptThing heroStand = Game->GetNearestWithScriptName(npc, "NPC_HeroStand");
                if (IsDistanceBetweenThingsUnder(Game->GetHero(), heroStand, 10.0f)) {
                    if (IsActiveThreadTerminating()) return;
                    Game->EntityTeleportToThing(npc, heroStand, false);
                    Game->EntitySetFacingAngle(npc, heroStand.GetAngleXY(), true);
                }
                CScriptThing camera = Game->GetNearestWithScriptName(npc, "NPC_Camera");
                if (camera.IsAlive()) { if (IsActiveThreadTerminating()) return; Game->CameraUseCameraPoint(camera, 0, 0, -1.0f, 0, -1); }
            }
            Game->EntitySetFacingAngleTowardsThing(npc, Game->GetHero(), false);
            if (Game->GetHealth(npc) > 0.0f) {
                Game->Speak("TEXT_QST_023_IDLE_03_HATE");
                while (Game->IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
            }
            Game->MoveToThing("MK_FI_HERO", 3.0f);
            Game->Pause(1.5f);
            Game->FadeScreenOut(1.5f, 1.5f, 0);
            Game->Pause(2.0f);
            Game->RemoveThing(npc, false, false);
            Game->CameraDefault();
            Log("pause-off");
        }
        Game->FadeScreenIn();
    }
};
static bool Run(int hitAt, int healAt, float health, bool nearStands, bool cameraAlive, int termAfter, const char* expected, long counter, bool runAway)
{
    CGame g; g.frames = 0; g.hitAt = hitAt; g.healAt = healAt; g.health = health; g.scriptedAt = 6; g.speakFrames = 0; g.nearStands = nearStands; g.cameraAlive = cameraAlive;
    CV_FishermanScript s; s.Game = &g; s.FishermanMustRunAway = false; s.termAfter = termAfter; s.termCalls = 0;
    RelationCounters[0] = RelationCounters[1] = RelationCounters[2] = 0;
    CScriptThing npc = { 4, true, 0.0f };
    g_log[0] = 0;
    s.NPCAttitudeProcess(npc);
    if (std::strcmp(g_log, expected) != 0) { std::printf("log: %s\n", g_log); return false; }
    return RelationCounters[2] == counter && s.FishermanMustRunAway == runAway;
}
int main()
{
    bool ok = true;
    ok = ok && Run(3, -1, 5.0f, true, true, 1000, "predicate;movie;pause-on;sheathe;teleport;face;teleport;face;camera;facehero;speak;TEXT_QST_023_IDLE_03_HATE;move;MK_FI_HERO;pause1.5;fadeout;pause2;remove;cameradefault;pause-off;fadein;", 2, true);
    ok = ok && Run(-1, 4, 5.0f, false, false, 1000, "predicate;movie;pause-on;sheathe;facehero;speak;TEXT_QST_023_IDLE_03_HATE;move;MK_FI_HERO;pause1.5;fadeout;pause2;remove;cameradefault;pause-off;fadein;", 2, true);
    ok = ok && Run(3, -1, 0.0f, true, true, 9, "predicate;", 0, false);    // hit at health <= 1: keeps waiting until terminated
    ok = ok && Run(3, -1, 5.0f, true, true, 2, "predicate;", 0, false);    // terminated before the hit
    if (!ok) { std::printf("00e7c010_TEST FAIL\n"); return 1; }
    std::printf("00e7c010_TEST PASS\n");
    return 0;
}
