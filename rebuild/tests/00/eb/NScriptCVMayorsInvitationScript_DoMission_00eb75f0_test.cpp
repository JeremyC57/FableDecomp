// Behaviour test for CV_MayorsInvitationScript::DoMission (0x00EB75F0): the call sequence through a logging game stub.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct C3DVector { float X, Y, Z; };
static C3DVector g_ZeroVector = { 0.0f, 0.0f, 0.0f };
static C3DVector g_MarkerPos = { 1.0f, 2.0f, 3.0f };
struct CScriptThing {
    const char* name;
    bool alive;
    const C3DVector* pos;
    CScriptThing() : name(""), alive(false), pos(0) {}
    bool IsAlive() const { return alive; }
    const C3DVector& GetPosition() const { return !pos ? g_ZeroVector : *pos; }
};
inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }
struct CGameScriptInterface {
    int framesUntilLoaded, framesUntilSuccess;
    bool ladyAlive, markerExists;
    bool* succeeded;
    const C3DVector* created;
    void NewScriptFrame() { Log("frame"); if (framesUntilSuccess-- == 0) *succeeded = true; }
    void ActivateQuest(const char* q) { Log(q); }
    bool IsLevelLoaded(const char* level) { Log(level); return framesUntilLoaded-- <= 0; }
    CScriptThing GetThingWithScriptName(const char* name)
    {
        CScriptThing t;
        t.name = name;
        t.alive = std::strcmp(name, "LadyGrey") == 0 && ladyAlive;
        if (std::strcmp(name, "MK_MI_LADY1") == 0 && markerExists) t.pos = &g_MarkerPos;
        return t;
    }
    CScriptThing CreateCreature(const char* def, const C3DVector& pos, const char* name, bool) { Log(def); Log(name); created = &pos; return CScriptThing(); }
    void SetThingAsUsable(const CScriptThing& t, bool usable) { Log(t.name); Log(usable ? "usable" : "locked"); }
};
struct CV_MayorsInvitationScript {
    CGameScriptInterface* Game;
    bool MissionSucceeded, MissionFailed;
    int terminateAt, checks;
    bool IsActiveThreadTerminating() { return ++checks == terminateAt; }
    void DoMission()
    {
        Game->ActivateQuest("V_ExposeMayor");
        while (!Game->IsLevelLoaded("BowerstonePosh")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing lady = Game->GetThingWithScriptName("LadyGrey");
        if (!IsThingAlive(lady)) {
            CScriptThing marker = Game->GetThingWithScriptName("MK_MI_LADY1");
            Game->CreateCreature("CREATURE_LADY_GREY", marker.GetPosition(), "LadyGrey", false);
        }
        CScriptThing door;
        door = Game->GetThingWithScriptName("GreyHouseFrontDoor");
        Game->SetThingAsUsable(door, false);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (MissionFailed) return;
        while (!MissionSucceeded) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->SetThingAsUsable(door, true);
    }
};
static CGameScriptInterface game;
static CV_MayorsInvitationScript s;
static bool Run(bool ladyAlive, bool marker, bool failed, int successAfter, const char* expect)
{
    g_log[0] = 0;
    game.framesUntilLoaded = 0;
    game.framesUntilSuccess = successAfter;
    game.ladyAlive = ladyAlive;
    game.markerExists = marker;
    game.created = 0;
    game.succeeded = &s.MissionSucceeded;
    s.Game = &game;
    s.MissionSucceeded = false;
    s.MissionFailed = failed;
    s.terminateAt = 0;
    s.checks = 0;
    s.DoMission();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    bool ok = Run(true, true, false, 1, "V_ExposeMayor;BowerstonePosh;GreyHouseFrontDoor;locked;frame;frame;GreyHouseFrontDoor;usable;")
        && Run(false, true, true, 9, "V_ExposeMayor;BowerstonePosh;CREATURE_LADY_GREY;LadyGrey;GreyHouseFrontDoor;locked;frame;")
        && game.created == &g_MarkerPos
        && Run(false, false, true, 9, "V_ExposeMayor;BowerstonePosh;CREATURE_LADY_GREY;LadyGrey;GreyHouseFrontDoor;locked;frame;")
        && game.created == &g_ZeroVector;
    if (!ok) { std::printf("00eb75f0_TEST FAIL\n"); return 1; }
    std::printf("00eb75f0_TEST PASS\n");
    return 0;
}